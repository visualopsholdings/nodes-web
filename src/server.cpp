/*
  server.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 26-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "json.hpp"
#include "cookie.hpp"
#include "session.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/core.hpp>
#include <restinio/router/express.hpp>

using router_t = restinio::router::express_router_t<>;

auto Server::handler()
{
  auto router = std::make_unique< router_t >();

	auto by = [&]( auto method ) {
		using namespace std::placeholders;
		return std::bind( method, this, _1, _2 );
	};

  router->http_get("", by(&Server::getroot));
  router->http_get("/", by(&Server::getroot));
  router->http_get("/fonts/:file", by(&Server::getfonts));
  router->http_get("/login", by(&Server::getlogin));
  router->http_get("/logout", by(&Server::getlogout));
  router->http_post("/login", by(&Server::postlogin));
  router->http_get("/rest/1.0/rawusers", by(&Server::getrawusers));
  router->http_get("/rest/1.0/users/me", by(&Server::getme));
  router->http_get("/rest/1.0/users/:id", by(&Server::getuser));
  router->http_get("/rest/1.0/streams/:id", by(&Server::getstream));
  router->http_get("/rest/1.0/streams", by(&Server::getstreams));
  router->http_get("/rest/1.0/conversations/:id", by(&Server::getconversation));
  router->http_get("/rest/1.0/streams/:id/policy/users", by(&Server::getstreampolicyusers));

  return router;
}

optional<shared_ptr<Session> > Server::getSession(const req_t& req) {

  if (!req->header().has_field("Cookie")) {
    BOOST_LOG_TRIVIAL(trace) << "no Cookie";  
    return {};
  }
  auto cookie = req->header().get_field("Cookie");
  auto id = Cookie::parseCookie(cookie);
  if (!id) {
    BOOST_LOG_TRIVIAL(trace) << "couldn't find id in cookie " << cookie;  
    return {};
  }
  if (!Sessions::instance()->has(id.value())) {
    BOOST_LOG_TRIVIAL(trace) << "couldn't find session id " << id.value();  
    return {};
  }
  return Sessions::instance()->get(id.value());

}

status_t Server::unauthorised(const req_t& req) {

  auto resp = init_resp(req->create_response(restinio::status_unauthorized()));
  json err = {
    { "status", 401 },
    { "err", "Username/Password incorrect" }
  };
  stringstream ss;
  ss << err;
  resp.set_body(ss.str());
  return resp.done();
}

bool Server::isAdmin(const req_t& req) {

  auto session = getSession(req);
  if (!session) {
    return false;
  }
  if (!session.value()->isAdmin()) {
    BOOST_LOG_TRIVIAL(trace) << "user is not admin " << session.value()->userid();  
    return false;
  }
  return true;
  
}

optional<string> Server::finishlogin(const string &password) {

  send({
    { "type", "login" },
    { "session", "1" },
    { "password", password }
  });
  json j = receive();
  BOOST_LOG_TRIVIAL(trace) << "login returned " << j;
  
  auto type = Json::getString(j, "type");
  if (!type) {
    BOOST_LOG_TRIVIAL(error) << "missing type in return";
    return nullopt;
  }
  
  if (type.value() == "err") {
    BOOST_LOG_TRIVIAL(error) << Json::getString(j, "msg").value();
    return nullopt;
  }

  return Sessions::instance()->create(j);
  
}

Server::Server(const string &reqConn): 
    _context(1), _req(_context, ZMQ_REQ) {
    
  _req.connect(reqConn);

}

void Server::run(int httpPort) {

  using traits_t =
      restinio::traits_t<
         restinio::asio_timer_manager_t,
         restinio::single_threaded_ostream_logger_t,
         router_t >;

  restinio::run(
    restinio::on_this_thread<traits_t>()
      .port(httpPort).address("localhost")
      .request_handler(handler())
  );
}

void Server::send(const json &json) {

  stringstream ss;
  ss << json;
  string m = ss.str();
  zmq::message_t msg(m.length());
  memcpy(msg.data(), m.c_str(), m.length());
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
  _req.send(msg);
#else
  _req.send(msg, zmq::send_flags::none);
#endif

}

json Server::receive() {

  zmq::message_t reply;
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
  _req.recv(&reply);
#else
  auto res = _req.recv(reply, zmq::recv_flags::none);
#endif
  string r((const char *)reply.data(), reply.size());
  return boost::json::parse(r);

}
