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
#include "zmqclient.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/core.hpp>
#include <restinio/router/express.hpp>
#include <restinio/websocket/websocket.hpp>

class boost_logger_t
{
	public:
		template< typename Message_Builder >
		void trace( Message_Builder && msg_builder )
		{
		  BOOST_LOG_TRIVIAL(trace) << msg_builder();
		}

		template< typename Message_Builder >
		void info( Message_Builder && msg_builder )
		{
		  BOOST_LOG_TRIVIAL(info) << msg_builder();
		}

		template< typename Message_Builder >
		void warn( Message_Builder && msg_builder )
		{
		  BOOST_LOG_TRIVIAL(warning) << msg_builder();
		}

		template< typename Message_Builder >
		void error( Message_Builder && msg_builder )
		{
		  BOOST_LOG_TRIVIAL(error) << msg_builder();
		}

};

using router_t = restinio::router::express_router_t<>;
using traits_t =
  restinio::traits_t<
    restinio::asio_timer_manager_t,
    boost_logger_t,
    router_t >;

Server::Server(int reqPort, int subPort, bool test): 
    _context(1), _req(_context, ZMQ_REQ), _test(test) {
    
  _req.connect("tcp://127.0.0.1:" + to_string(reqPort));
  
	_zmq = shared_ptr<ZMQClient>(new ZMQClient(this, subPort));
	
}

namespace nodes {

status_t getchat(Server *server, const req_t& req, params_t );
status_t getrawusers(Server *server, const req_t& req, params_t );
status_t getstreams(Server *server, const req_t& req, params_t );
status_t getstream(Server *server, const req_t& req, params_t );
status_t getconversation(Server *server, const req_t& req, params_t );
status_t getstreampolicyusers(Server *server, const req_t& req, params_t );
status_t getme(Server *server, const req_t& req, params_t );
status_t getlogin(Server *server, const req_t& req, params_t );
status_t postlogin(Server *server, const req_t& req, params_t );
status_t getlogout(Server *server, const req_t& req, params_t );
status_t getuser(Server *server, const req_t& req, params_t );
status_t posttyping(Server *server, const req_t& req, params_t );
status_t getinfos(Server *server, const req_t& req, params_t );
status_t getrawsites(Server *server, const req_t& req, params_t );
status_t postusers(Server *server, const req_t& req, params_t );
status_t websocket(Server *server, const req_t& req, params_t );
status_t getrawgroups(Server *server, const req_t& req, params_t );
status_t getrawgroup(Server *server, const req_t& req, params_t );
status_t getrawgroupusers(Server *server, const req_t& req, params_t );
status_t getusers(Server *server, const req_t& req, params_t );
status_t postgroupusers(Server *server, const req_t& req, params_t );
status_t deletegroupusers(Server *server, const req_t& req, params_t );
status_t getrawuser(Server *server, const req_t& req, params_t );
status_t getgroups(Server *server, const req_t& req, params_t );
status_t getgroupusers(Server *server, const req_t& req, params_t );
status_t getrawstreams(Server *server, const req_t& req, params_t );
status_t getrawstream(Server *server, const req_t& req, params_t );
status_t getrawstreampolicy(Server *server, const req_t& req, params_t );
status_t getrawgrouppolicy(Server *server, const req_t& req, params_t );
status_t deletestream(Server *server, const req_t& req, params_t );
status_t deletegroup(Server *server, const req_t& req, params_t );

status_t getroot(Server *server, const req_t& req, params_t params)
{
  auto resp = req->create_response(restinio::status_found());
	resp.append_header("Location", "/apps/chat/#/");
  return resp.done();
  
}

status_t servefile(const req_t& req, const string &filename, optional<string> type = nullopt) {

  BOOST_LOG_TRIVIAL(trace) << filename;

  auto resp = req->create_response();
  resp.set_body(restinio::sendfile(filename));
  if (type) {
    resp.append_header("Content-Type", type.value());
  }
  
  return resp.done();

}

string getmime(const string &file) {

  if (file.find(".js") != string::npos) {
    return "application/javascript";
  }
  else if (file.find(".css") != string::npos) {
    return "text/css";
  }
  else if (file.find(".svg") != string::npos) {
    return "image/svg+xml";
  }
  else {
    return "text/text";
  }

}

status_t getfonts(Server *server, const req_t& req, params_t params)
{
  const auto file = restinio::cast_to<string>( params[ "file" ] );

  string filename;
  if (!server->_test) {
    filename = "../";
  }
  filename += "frontend/fonts/" + file;
  return servefile(req, filename);

}

status_t getappindex(Server *server, const req_t& req, params_t params)
{
  const auto app = restinio::cast_to<string>( params[ "app" ] );

  return servefile(req, "frontend/apps/" + app + "/index.html");

}

status_t getappfile(Server *server, const req_t& req, params_t params)
{
  const auto app = restinio::cast_to<string>( params[ "app" ] );
  const auto file = restinio::cast_to<string>( params[ "file" ] );

  return servefile(req, "frontend/apps/" + app + "/" + file, getmime(file));

}

status_t getappasset(Server *server, const req_t& req, params_t params)
{
  const auto app = restinio::cast_to<string>(params["app"]);
  const auto file = restinio::cast_to<string>(params["file"]);

  return servefile(req, "frontend/apps/" + app + "/assets/" + file, getmime(file));

}

};

auto Server::handler()
{
  auto router = std::make_unique< router_t >();

	auto by = [&]( auto method ) {
		using namespace std::placeholders;
		return std::bind( method, this, _1, _2 );
	};

  router->http_get("", by(&nodes::getroot));
  router->http_get("/", by(&nodes::getroot));
  router->http_get("/fonts/:file", by(&nodes::getfonts));
  router->http_get("/login", by(&nodes::getlogin));
  
  if (_test) {
    // we don't install these handlers during production. Nginx takes
    // care of all this.
    router->http_get("/apps/:app/", by(&nodes::getappindex));
    router->http_get("/apps/:app/:file", by(&nodes::getappfile));
    router->http_get("/apps/:app/assets/:file", by(&nodes::getappasset));
  }
  
  router->http_get("/logout", by(&nodes::getlogout));
  router->http_post("/login", by(&nodes::postlogin));
  router->http_get("/rest/1.0/rawusers", by(&nodes::getrawusers));
  router->http_get("/rest/1.0/users/me", by(&nodes::getme));
  router->http_get("/rest/1.0/users/:id", by(&nodes::getuser));
  router->http_get("/rest/1.0/streams/:id", by(&nodes::getstream));
  router->http_get("/rest/1.0/streams", by(&nodes::getstreams));
  router->http_get("/rest/1.0/conversations/:id", by(&nodes::getconversation));
  router->http_get("/rest/1.0/streams/:id/policy/users", by(&nodes::getstreampolicyusers));
  router->http_post("/rest/1.0/ideas", [&](const req_t& req, params_t params) {
    return sendBodyReturnEmptyObj(req, "message", true);
  });
  router->http_post("/rest/1.0/users/me/typing", by(&nodes::posttyping));
  router->http_get("/rest/1.0/infos", by(&nodes::getinfos));
  router->http_post("/rest/1.0/infos", [&](const req_t& req, params_t params) {
    return sendBodyReturnEmptyObj(req, "setinfo", true);
  });
  router->http_get("/rest/1.0/rawsites", by(&nodes::getrawsites));
  router->http_put("/rest/1.0/sites", [&](const req_t& req, params_t params) {
    return sendBodyReturnEmptyObj(req, "setsite", true);
  });
  router->http_post("/rest/1.0/users", by(&nodes::postusers));
  router->http_get("/websocket", by(&nodes::websocket));
  router->http_get("/rest/1.0/rawgroups", by(&nodes::getrawgroups));
  router->http_get("/rest/1.0/rawgroups/:id", by(&nodes::getrawgroup));
  router->http_get("/rest/1.0/rawgroups/:id/users", by(&nodes::getrawgroupusers));
  router->http_get("/rest/1.0/users", by(&nodes::getusers));
  router->http_post("/rest/1.0/groups/:id/users", by(&nodes::postgroupusers));
  router->http_delete("/rest/1.0/groups/:id/users/:user", by(&nodes::deletegroupusers));
  router->http_get("/rest/1.0/rawusers/:id", by(&nodes::getrawuser));
  router->http_put("/rest/1.0/users/:id", [&](const req_t& req, params_t params) {
    return sendBodyReturnEmptyObj(req, "setuser", true, restinio::cast_to<string>(params["id"]));
  });
  router->http_get("/rest/1.0/groups", by(&nodes::getgroups));
  router->http_get("/rest/1.0/groupusers", by(&nodes::getgroupusers));

  router->http_get("/rest/1.0/rawstreams", by(&nodes::getrawstreams));
  router->http_get("/rest/1.0/rawstreams/:id", by(&nodes::getrawstream));
  router->http_get("/rest/1.0/rawstreams/:id/policy", by(&nodes::getrawstreampolicy));
  router->http_put("/rest/1.0/rawstreams/:id/policy", [&](const req_t& req, params_t params) {
    return sendBodyReturnEmptyObj(req, "setstreampolicy", true, restinio::cast_to<string>(params["id"]));
  });
  router->http_post("/rest/1.0/streams", [&](const req_t& req, params_t params) {
    return sendBodyReturnEmptyObj(req, "addstream", false);
  });
  router->http_delete("/rest/1.0/streams/:id", by(&nodes::deletestream));

  router->http_get("/rest/1.0/rawgroups/:id/policy", by(&nodes::getrawgrouppolicy));
  router->http_put("/rest/1.0/rawgroups/:id/policy", [&](const req_t& req, params_t params) {
    return sendBodyReturnEmptyObj(req, "setgrouppolicy", true, restinio::cast_to<string>(params["id"]));
  });
  router->http_post("/rest/1.0/groups", [&](const req_t& req, params_t params) {
    return sendBodyReturnEmptyObj(req, "addgroup", false);
  });
  router->http_put("/rest/1.0/groups/:id", [&](const req_t& req, params_t params) {
    return sendBodyReturnEmptyObj(req, "setgroup", false, restinio::cast_to<string>(params["id"]));
  });
  router->http_put("/rest/1.0/streams/:id", [&](const req_t& req, params_t params) {
    return sendBodyReturnEmptyObj(req, "setstream", false, restinio::cast_to<string>(params["id"]));
  });
  router->http_delete("/rest/1.0/groups/:id", by(&nodes::deletegroup));

  return router;
}

void Server::run(int httpPort) {

  _zmq->run();
  
  restinio::run(
    restinio::on_this_thread<traits_t>()
      .port(httpPort).address("localhost")
      .request_handler(handler())
  );
}

shared_ptr<rws::ws_t> Server::createWS(const req_t& req) {

  return rws::upgrade< traits_t >(
      *req,
      rws::activation_t::immediate,
      [](shared_ptr<rws::ws_t> wsh, shared_ptr<rws::message_t> m){
      
        BOOST_LOG_TRIVIAL(trace) << "got " << m->payload();;
        
      } );

}

void Server::sendWS(uint64_t &id, const json &json) {

  BOOST_LOG_TRIVIAL(trace) << "sendWS " << json;

  auto i = _registry.find(id);
  if (i != _registry.end()) {
    stringstream ss;
    ss << json;
    i->second->send_message(rws::message_t(rws::final_frame, rws::opcode_t::text_frame, ss.str()));
  }
}

void Server::sendAllWS(const json &json) {

  BOOST_LOG_TRIVIAL(trace) << "sendAllWS " << json;

  stringstream ss;
  ss << json;
  for (auto i: _registry) {
    try {
      i.second->send_message(rws::message_t(rws::final_frame, rws::opcode_t::text_frame, ss.str()));
    }
    catch (...) {
      BOOST_LOG_TRIVIAL(error) << "failed send_message on socket";      
    }
  }
  
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

status_t Server::fatal(const req_t& req, const string &msg) {

  BOOST_LOG_TRIVIAL(error) << "fatal " << msg;

  auto resp = init_resp(req->create_response(restinio::status_internal_server_error()));
  json err = {
    { "status", 500 },
    { "err", msg }
  };
  stringstream ss;
  ss << err;
  resp.set_body(ss.str());
  return resp.done();
}

status_t Server::warning(const req_t& req, const string &msg) {

  BOOST_LOG_TRIVIAL(error) << "warning " << msg;

  auto resp = init_resp(req->create_response(restinio::status_bad_request()));
  json err = {
    { "status", 400 },
    { "err", msg }
  };
  stringstream ss;
  ss << err;
  resp.set_body(ss.str());
  return resp.done();
}

status_t Server::returnEmptyObj(const req_t& req) {

  auto resp = req->create_response();
  resp.set_body("{}");
  return resp.done();
  
}

status_t Server::checkErrorsReturnEmptyObj(const req_t& req, json &j, const string &type) {

  // test for an error...
  auto rettype = Json::getString(j, "type");
  if (!rettype) {
    BOOST_LOG_TRIVIAL(error) << type << " missing type in return";
    return fatal(req, "missing type");
  }
  if (rettype.value() == "err") {
    auto msg = Json::getString(j, "msg");
    auto level = Json::getString(j, "level");
    if (level.value() == "warning") {
      return warning(req, msg.value());
    }
    return fatal(req, msg.value());
  }
  
  return returnEmptyObj(req);

}

status_t Server::sendBodyReturnEmptyObj(const req_t& req, const string &type, bool admin, optional<string> id) {

  auto session = getSession(req);
  if (!session) {
    return unauthorised(req);
  }

  json j = boost::json::parse(req->body());
//  BOOST_LOG_TRIVIAL(trace) << type << " " << j;

  if (admin) {
    // TBD: This has no end to end test.
    if (!session.value()->isAdmin()) {
      return unauthorised(req);
    }
  }
  else {
    j.as_object()["me"] = session.value()->userid();
  }

  if (!j.is_object()) {
    return fatal(req, "body is not object");
  }

  j.as_object()["type"] = type;
  if (j.as_object().if_contains("_id")) {
    j.as_object()["id"] = Json::getString(j, "_id").value();
    j.as_object().erase("_id");
  }
  
  if (id) {
    j.as_object()["id"] = id.value();
  }
  
	send(j);
  j = receive();
  
//  BOOST_LOG_TRIVIAL(trace) << j;
  
  return checkErrorsReturnEmptyObj(req, j, type);

}

status_t Server::receiveArray(const req_t& req, const string &field) {

  json j = receive();

  auto result = Json::getArray(j, field);
  
  if (!result) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << field << " missing";
    return init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

//  BOOST_LOG_TRIVIAL(debug) << result.value();

  auto resp = init_resp( req->create_response() );

  stringstream ss;
  ss << Json::fixIds(result.value());
  resp.set_body(ss.str());

  return resp.done();

}

status_t Server::receiveObject(const req_t& req, const string &field) {

  json j = receive();

  auto result = Json::getObject(j, field);
  
  if (!result) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << field << " missing";
    return init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

//  BOOST_LOG_TRIVIAL(debug) << result.value();

  auto resp = init_resp( req->create_response() );

  stringstream ss;
  ss << Json::fixObject(result.value());
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
