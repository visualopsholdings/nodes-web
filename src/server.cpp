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
#include "etag.hpp"
#include "path.hpp"

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

Server::Server(int reqPort, int subPort, const string &mediaDir, bool test): 
    _context(1), _req(_context, ZMQ_REQ), _test(test) {
    
  _mediaDir = mediaDir;
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
status_t getstreampolicygroups(Server *server, const req_t& req, params_t );
status_t getme(Server *server, const req_t& req, params_t );
status_t getlogin(Server *server, const req_t& req, params_t );
status_t postlogin(Server *server, const req_t& req, params_t );
status_t getlogout(Server *server, const req_t& req, params_t );
status_t getuser(Server *server, const req_t& req, params_t );
status_t posttyping(Server *server, const req_t& req, params_t );
status_t getinfos(Server *server, const req_t& req, params_t );
status_t getrawsites(Server *server, const req_t& req, params_t );
status_t getsites(Server *server, const req_t& req, params_t );
status_t postnewuser(Server *server, const req_t& req, params_t );
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
status_t getstreamsharelink(Server *server, const req_t& req, params_t );
status_t postideas(Server *server, const req_t& req, params_t );
status_t getcanreg(Server *server, const req_t& req, params_t );
status_t deleterawuser(Server *server, const req_t& req, params_t );
status_t getgroup(Server *server, const req_t& req, params_t );
status_t getgrouppolicy(Server *server, const req_t& req, params_t );
status_t getnodes(Server *server, const req_t& req, params_t );
status_t getnode(Server *server, const req_t& req, params_t );
status_t deletenode(Server *server, const req_t& req, params_t );
status_t postgroups(Server *server, const req_t& req, params_t );
status_t poststreams(Server *server, const req_t& req, params_t );
status_t postrawgroups(Server *server, const req_t& req, params_t );
status_t postrawstreams(Server *server, const req_t& req, params_t );
status_t postrawusers(Server *server, const req_t& req, params_t );
status_t deleteidea(Server *server, const req_t& req, params_t );
status_t getideaspurgecount(Server *server, const req_t& req, params_t );
status_t postrawstream(Server *server, const req_t& req, params_t );
status_t postmediaupload(Server *server, const req_t& req, params_t );
status_t getcontent(Server *server, const req_t& req, params_t );
status_t getrawmedia(Server *server, const req_t& req, params_t );
status_t deletemedia(Server *server, const req_t& req, params_t );
status_t postrawmedia(Server *server, const req_t& req, params_t );

status_t getroot(Server *server, const req_t& req, params_t params)
{
  auto resp = req->create_response(restinio::status_found());
	resp.append_header("Location", "/apps/chat/#/");
  return resp.done();
  
}

status_t servefile(const req_t& req, const string &filename, optional<string> type = nullopt) {

  BOOST_LOG_TRIVIAL(trace) << filename << " as " << (type ? type.value() : "??");

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

  return servefile(req, "frontend/apps/" + app + "/index.html", "text/html");

}

};

std::unique_ptr<router_t> Server::createRouter()
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
    router->http_get("/apps/:app", by(&nodes::getappindex));
    router->http_get("/apps/:app/", by(&nodes::getappindex));
  }
  
  router->http_get("/logout", by(&nodes::getlogout));
  router->http_post("/login", by(&nodes::postlogin));
  router->http_get("/rest/1.0/rawusers/purgecount", [&](const req_t& req, params_t params) {
    auto msg = dictO({
      { "type", "purgecountusers" }
    });
    return sendSimpleReturnRawObjectAdmin(msg, req);
  });
  router->http_get("/rest/1.0/rawusers", by(&nodes::getrawusers));
  router->http_post("/rest/1.0/rawusers", by(&nodes::postrawusers));
  router->http_get("/rest/1.0/users/me", by(&nodes::getme));
  router->http_get("/rest/1.0/users/:id", by(&nodes::getuser));
  router->http_get("/rest/1.0/streams/:id", by(&nodes::getstream));
  router->http_get("/rest/1.0/streams", by(&nodes::getstreams));
  router->http_get("/rest/1.0/conversations/:id", by(&nodes::getconversation));
  router->http_get("/rest/1.0/streams/:id/policy/users", by(&nodes::getstreampolicyusers));
  router->http_get("/rest/1.0/streams/:id/policy/groups", by(&nodes::getstreampolicygroups));
  router->http_get("/rest/1.0/streams/:id/sharelink", by(&nodes::getstreamsharelink));
  router->http_post("/rest/1.0/ideas", by(&nodes::postideas));
  router->http_delete("/rest/1.0/ideas/:id", by(&nodes::deleteidea));
  router->http_post("/rest/1.0/users/me/typing", by(&nodes::posttyping));
  router->http_get("/rest/1.0/infos", by(&nodes::getinfos));
  router->http_post("/rest/1.0/infos", [&](const req_t& req, params_t params) {
    return sendObjReturnEmptyObjAdmin(req, dictO({
      { "type", "setinfo" }
    }));
  });
  router->http_get("/rest/1.0/rawsites", by(&nodes::getrawsites));
  router->http_put("/rest/1.0/sites", [&](const req_t& req, params_t params) {
    return sendObjReturnEmptyObjAdmin(req, dictO({
      { "type", "setsite" }
    }));
  });
  router->http_get("/rest/1.0/sites", by(&nodes::getsites));
  router->http_post("/rest/1.0/users/new", by(&nodes::postnewuser));
  router->http_post("/rest/1.0/users", by(&nodes::postusers));
  router->http_get("/websocket", by(&nodes::websocket));
  router->http_get("/rest/1.0/rawgroups", by(&nodes::getrawgroups));
  router->http_get("/rest/1.0/rawgroups/purgecount", [&](const req_t& req, params_t params) {
    auto msg = dictO({
      { "type", "purgecountgroups" }
    });
    return sendSimpleReturnRawObjectAdmin(msg, req);
  });
  router->http_get("/rest/1.0/rawgroups/:id", by(&nodes::getrawgroup));
  router->http_get("/rest/1.0/rawgroups/:id/users", by(&nodes::getrawgroupusers));
  router->http_post("/rest/1.0/rawgroups", by(&nodes::postrawgroups));
  router->http_get("/rest/1.0/users", by(&nodes::getusers));
  router->http_post("/rest/1.0/groups/:id/users", by(&nodes::postgroupusers));
  router->http_delete("/rest/1.0/groups/:id/users/:user", by(&nodes::deletegroupusers));
  router->http_get("/rest/1.0/rawusers/:id", by(&nodes::getrawuser));
  router->http_put("/rest/1.0/users/:id", [&](const req_t& req, params_t params) {
    return sendObjReturnEmptyObjAdmin(req, dictO({
      { "type", "setuser" }
    }), restinio::cast_to<string>(params["id"]));
  });
  router->http_get("/rest/1.0/groups", by(&nodes::getgroups));
  router->http_get("/rest/1.0/groups/:id", by(&nodes::getgroup));
  router->http_get("/rest/1.0/groups/:id/users", by(&nodes::getgroupusers));
  router->http_get("/rest/1.0/groups/:id/policy", by(&nodes::getgrouppolicy));

  router->http_get("/rest/1.0/rawstreams", by(&nodes::getrawstreams));
  router->http_get("/rest/1.0/rawstreams/purgecount", [&](const req_t& req, params_t params) {
    auto msg = dictO({
      { "type", "purgecount" },
      { "objtype", "stream" }
    });
    return sendSimpleReturnRawObjectAdmin(msg, req);
  });
  router->http_get("/rest/1.0/rawstreams/:id", by(&nodes::getrawstream));
  router->http_get("/rest/1.0/rawstreams/:id/policy", by(&nodes::getrawstreampolicy));
  router->http_put("/rest/1.0/rawstreams/:id/policy", [&](const req_t& req, params_t params) {
    return sendObjReturnEmptyObjAdmin(req, dictO({
      { "type", "setobjectpolicy" },
      { "objtype", "stream" }
    }), restinio::cast_to<string>(params["id"]));
  });
  router->http_get("/rest/1.0/rawstreams/:id/purgecount", by(&nodes::getideaspurgecount));
  router->http_post("/rest/1.0/rawstreams", by(&nodes::postrawstreams));
  router->http_post("/rest/1.0/rawstreams/:id", by(&nodes::postrawstream));
  router->http_post("/rest/1.0/streams", by(&nodes::poststreams));
  router->http_delete("/rest/1.0/streams/:id", by(&nodes::deletestream));

  router->http_get("/rest/1.0/rawmedia", by(&nodes::getrawmedia));
  router->http_get("/rest/1.0/rawmedia/purgecount", [&](const req_t& req, params_t params) {
    auto msg = dictO({
      { "type", "purgecount" },
      { "objtype", "media" }
    });
    return sendSimpleReturnRawObjectAdmin(msg, req);
  });
  router->http_delete("/rest/1.0/rawmedia/:id", by(&nodes::deletemedia));
  router->http_post("/rest/1.0/rawmedia", by(&nodes::postrawmedia));

  router->http_get("/rest/1.0/rawgroups/:id/policy", by(&nodes::getrawgrouppolicy));
  router->http_put("/rest/1.0/rawgroups/:id/policy", [&](const req_t& req, params_t params) {
    return sendObjReturnEmptyObjAdmin(req, dictO({
      { "type", "setgrouppolicy" }
    }), restinio::cast_to<string>(params["id"]));
  });
  router->http_post("/rest/1.0/groups", by(&nodes::postgroups));
  router->http_put("/rest/1.0/groups/:id", [&](const req_t& req, params_t params) {
    return sendObjReturnEmptyObj(req, dictO({
      { "type", "setgroup" }
    }), restinio::cast_to<string>(params["id"]));
  });
  router->http_put("/rest/1.0/streams/:id", [&](const req_t& req, params_t params) {
    return sendObjReturnEmptyObj(req, dictO({
      { "type", "setobject" },
      { "objtype", "stream" }
    }), restinio::cast_to<string>(params["id"]));
  });
  router->http_delete("/rest/1.0/groups/:id", by(&nodes::deletegroup));
  router->http_get("/rest/1.0/users/canreg/:token", by(&nodes::getcanreg));
  router->http_delete("/rest/1.0/rawusers/:id", by(&nodes::deleterawuser));
  
  router->http_get("/rest/1.0/nodes", by(&nodes::getnodes));
  router->http_get("/rest/1.0/nodes/:id", by(&nodes::getnode));
  router->http_post("/rest/1.0/nodes", [&](const req_t& req, params_t params) {
    return sendObjReturnEmptyObjAdmin(req, dictO({
      { "type", "addnode" }
    }));
  });
  router->http_delete("/rest/1.0/nodes/:id", by(&nodes::deletenode));
  router->http_post("/rest/1.0/media/upload/:id", by(&nodes::postmediaupload));
  router->http_get("/rest/1.0/media/:id/content", by(&nodes::getcontent));

  router->non_matched_request_handler([&](const req_t& req) {
  
    string path(req->header().path());
    
    BOOST_LOG_TRIVIAL(warning) << "no matched request " << path;
    
    if (_test) {
      // we don't do this during production. Nginx takes
      // care of all this.
      auto appPath = Path::getAppPath(path);
      if (appPath) {
        return nodes::servefile(req, appPath.value(), nodes::getmime(path));
      }
    }
    
    return req->create_response(restinio::status_not_found()).connection_close().done();
  });

  return router;
}

void Server::run(int httpPort) {

  _zmq->run();
  
  restinio::run(
    restinio::on_this_thread<traits_t>()
      .port(httpPort).address("localhost")
      .request_handler(createRouter())
  );
}

shared_ptr<rws::ws_t> Server::createWS(const req_t& req) {

  auto ws = rws::upgrade< traits_t >(
    *req,
    rws::activation_t::immediate,
    [this](shared_ptr<rws::ws_t> wsh, shared_ptr<rws::message_t> m){
    
      if( rws::opcode_t::connection_close_frame == m->opcode() ) {
        BOOST_LOG_TRIVIAL(trace) << "closing " <<wsh->connection_id();
        _registry.erase(wsh->connection_id());
      }
      else {
        BOOST_LOG_TRIVIAL(trace) << "got " << m->payload();;
      }
      
  });

  // save ID:    
  _registry.emplace(ws->connection_id(), ws);

  return ws;

}

void Server::sendWS(uint64_t &id, const DictO &json) {

  auto s = Dict::toString(json);
  BOOST_LOG_TRIVIAL(trace) << "sendWS " << s;

  auto i = _registry.find(id);
  if (i != _registry.end()) {
    i->second->send_message(rws::message_t(rws::final_frame, rws::opcode_t::text_frame, s));
  }
}

void Server::sendAllWS(const DictO &json) {

  auto s = Dict::toString(json, false);
  BOOST_LOG_TRIVIAL(trace) << "sendAllWS " << s;

  for (auto i: _registry) {
    try {
      i.second->send_message(rws::message_t(rws::final_frame, rws::opcode_t::text_frame, s));
    }
    catch (...) {
      BOOST_LOG_TRIVIAL(error) << "failed send_message on socket";      
    }
  }
  
}

void Server::sendAllWSExcept(const DictO &json, const string &socketid) {

  auto s = Dict::toString(json, false);
  BOOST_LOG_TRIVIAL(trace) << "sendAllWSExcept " << s << " not " << socketid;

  long sock = 0;
  try {
    sock = stol(socketid);
  } catch(...) {
    BOOST_LOG_TRIVIAL(trace) << "skipping socketid, not a number";      
  }
  
  for (auto i: _registry) {
    if (i.first == sock) {
      continue;
    }
    try {
      i.second->send_message(rws::message_t(rws::final_frame, rws::opcode_t::text_frame, s));
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
  auto err = dictO({
    { "status", 401 },
    { "err", "Username/Password incorrect" }
  });
  resp.set_body(Dict::toString(err, false));
  return resp.done();
}

status_t Server::not_modified(const req_t& req, optional<string> etag) {

  auto resp = init_resp(req->create_response(restinio::status_not_modified()));
  auto err = dictO({
    { "status", 403 },
    { "err", "Not modified" }
  });
  resp.set_body(Dict::toString(err, false));
  if (etag) {
    resp.append_header("Etag", etag.value());
  }
  return resp.done();

}

status_t Server::fatal(const req_t& req, const string &msg) {

  BOOST_LOG_TRIVIAL(error) << "fatal " << msg;

  auto resp = init_resp(req->create_response(restinio::status_internal_server_error()));
  auto err = dictO({
    { "status", 500 },
    { "err", msg }
  });
  resp.set_body(Dict::toString(err, false));
  return resp.done();
}

status_t Server::warning(const req_t& req, const string &msg) {

  BOOST_LOG_TRIVIAL(error) << "warning " << msg;

  auto resp = init_resp(req->create_response(restinio::status_bad_request()));
  auto err = dictO({
    { "status", 400 },
    { "err", msg }
  });
  resp.set_body(Dict::toString(err, false));
  return resp.done();
}

status_t Server::security(const req_t& req) {

  BOOST_LOG_TRIVIAL(error) << "security";

  auto resp = init_resp(req->create_response(restinio::status_unauthorized()));
  auto err = dictO({
    { "status", 401 },
    { "err", "Unauthorized" }
  });
  resp.set_body(Dict::toString(err, false));
  return resp.done();
}

status_t Server::returnEmptyObj(const req_t& req, shared_ptr<ETagHandler> etag) {

  auto resp = req->create_response();
  resp.set_body("{}");
  etag->setHeaders(resp);
  return resp.done();
  
}

status_t Server::returnEmptyArray(const req_t& req, shared_ptr<ETagHandler> etag) {

  auto resp = req->create_response();
  resp.set_body("[]");
  etag->setHeaders(resp);
  return resp.done();
  
}

status_t Server::returnObj(const req_t& req, shared_ptr<ETagHandler> etag, const DictG &msg) {

  auto s = Dict::toString(msg, false);
  BOOST_LOG_TRIVIAL(trace) << s;
  
  auto resp = req->create_response();
  resp.set_body(s);
  etag->setHeaders(resp);
  return resp.done();
  
}

optional<status_t> Server::checkErrors(const req_t& req, const DictO &msg, const string &type) {

  // test for an error...
  auto rettype = Dict::getString(msg, "type");
  if (!rettype) {
    BOOST_LOG_TRIVIAL(error) << type << " missing type in return " << Dict::toString(msg);
    return fatal(req, "missing type");
  }
  if (rettype.value() == "err") {
    auto m = Dict::getString(msg, "msg");
    auto level = Dict::getString(msg, "level");
    if (level.value() == "warning") {
      return warning(req, m.value());
    }
    else if (level.value() == "security") {
      return security(req);
    }
    return fatal(req, m.value());
  }

  return nullopt;
  
}

DictO Server::callNodes(const DictO &obj) {

  const auto format = ".json";
  
  string m = Dict::toString(obj, false, format);

  BOOST_LOG_TRIVIAL(trace) << "sending " << m;
  
  zmq::message_t msg(m.length());
  memcpy(msg.data(), m.c_str(), m.length());
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
  _req.send(msg);
#else
  _req.send(msg, zmq::send_flags::none);
#endif
  
  zmq::message_t reply;
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
  _req.recv(&reply);
#else
  auto res = _req.recv(reply, zmq::recv_flags::none);
#endif
  string r((const char *)reply.data(), reply.size());
  BOOST_LOG_TRIVIAL(trace) << "received " << r;

  auto oreply = Dict::getObject(Dict::parseString(r, format));
  if (!oreply) {
    BOOST_LOG_TRIVIAL(error) << "didn't get back JSON object " << r;
    return DictO();
  }
  
  
  return *oreply;
  
}

status_t Server::sendObjReturnEmptyObj(const req_t& req, const DictO &msg, optional<string> id) {

  // all handlers should look very similar to this one, so here is where
  // we document how it all works.
  
  // The entry point for REST is right here, so first make sure there is a session.
  auto session = getSession(req);
  if (!session) {
    return unauthorised(req);
  }

  // parse out the body of the request to an object, if you need that.
  auto body = Dict::getObject(Dict::parseString(req->body()));
  if (!body) {
    return fatal(req, "could not parse body to JSON.");
  }
//  BOOST_LOG_TRIVIAL(trace) << Dict::toString(*body);

  // set "me" into the call, you can get that from the session.
  (*body)["me"] = session.value()->userid();

  // create a new message that you will send on....
  // here we merge the "msg" info the object and an in the id if it exists.
  // this would be different to every nodes call obviously.
  auto newbody = mergeBody(req, *body, msg, id);
  
  // you would probably hard code the type for "checkErrors" but we get it from the
  // message being sent. 
  auto type = Dict::getString(newbody, "type");
  if (!type) {
    return fatal(req, "body missing type");
  }

  // send off the request to nodes and get something back.
  auto j = callNodes(newbody);
  
  // always do this, it checks for any errors in the response and creates
  // a web response for that. pass in something useful to "type" so the error
  // has some context.
  auto resp = checkErrors(req, j, *type);
  if (resp) {
    return resp.value();
  }
  
  // and return something back to the web call if required. We just return an empty object here.
  return returnEmptyObj(req, ETag::none());
  
}

status_t Server::sendObjReturnEmptyObjAdmin(const req_t& req, const DictO &msg, optional<string> id) {

  // this is VERY similar to above, we just check for the admin flag on the user and fail.
  // we don't set "me" like above.
  
  if (!isAdmin(req)) {
    return unauthorised(req);
  }

  auto body = Dict::getObject(Dict::parseString(req->body()));
  if (!body) {
    return fatal(req, "could not parse body to JSON.");
  }
//  BOOST_LOG_TRIVIAL(trace) << Dict::toString(*body);

  auto newbody = mergeBody(req, *body, msg, id);
  
  auto type = Dict::getString(newbody, "type");
  if (!type) {
    return fatal(req, "body missing type");
  }

  auto j = callNodes(newbody);
  
  auto resp = checkErrors(req, j, *type);
  if (resp) {
    return resp.value();
  }
  
  return returnEmptyObj(req, ETag::none());

}

DictO Server::mergeBody(const req_t& req, const DictO &body, const DictO &msg, optional<string> id) {

  DictO obj;
  
  // we can't erase, so just copy all fields instead of the _id, if we have one
  // set the correct field name;
  for (auto i: body) {
    auto key = get<0>(i);
    auto value = get<1>(i);
    if (key == "_id") {
      auto id = Dict::getString(value);
      if (!id) {
        BOOST_LOG_TRIVIAL(error) << "_id is not a string" << Dict::toString(value);
        continue;
      }
      // use "id" as the field name.
      obj["id"] = *id;
    }
    else {
      obj[key] =  get<1>(i);
    }
  }
  
  // and forget all that if the actually pass the id.
  if (id) {
    obj["id"] = *id;
  }
  
  // copy fields of the msg into the body. Could do it the other way around.
  string type = "??";
  for (auto i: msg) {
    auto key = get<0>(i);
    auto value = get<1>(i);
    if (key == "type") {
      auto t = Dict::getString(value);
      if (!t) {
        BOOST_LOG_TRIVIAL(error) << "type is not a string" << Dict::toString(value);
        continue;
      }
      type = *t;
    }
    else {
      obj[key] = value;
    }
  }
  
  obj["type"] = type;
  
  // if we have a socket id, then send it on as the correlation id.
  if (req->header().has_field("socketid")) {
    BOOST_LOG_TRIVIAL(trace) << "setting socket id from header";
    obj["corr"] = string(req->header().value_of("socketid"));
  }

  return obj;
  
}

status_t Server::sendSimpleReturnRawObjectAdmin(const DictO &msg, const req_t& req) {

  if (!isAdmin(req)) {
    return unauthorised(req);
  }

  auto type = Dict::getString(msg, "type");
  if (!type) {
    return fatal(req, "msg missing type");
  }

  auto j = callNodes(msg);

  auto resp = checkErrors(req, j, *type);
  if (resp) {
    return resp.value();
  }

  return returnObj(req, ETag::none(), j);

}

status_t Server::sendSimpleReturnEmptyObjAdmin(const DictO &msg, const req_t& req) {

  if (!isAdmin(req)) {
    return unauthorised(req);
  }
  
  auto type = Dict::getString(msg, "type");
  if (!type) {
    return fatal(req, "msg missing type");
  }

  auto j = callNodes(msg);
  
  auto resp = checkErrors(req, j, *type);
  if (resp) {
    return resp.value();
  }

  return returnEmptyObj(req, ETag::none());

}

status_t Server::returnArray(const req_t& req, shared_ptr<ETagHandler> etag, const DictO &obj, const string &field) {

  // some etags don't use the result so they just always return false.
  if (etag->resultModified(obj, field)) {
    return not_modified(req, etag->origEtag());
  }

  auto result = Dict::getVector(obj, field);
  
  if (!result) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << field << " missing";
    return init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

//  BOOST_LOG_TRIVIAL(debug) << result.value();

  auto msg = Json::fixIds(result.value());
  return returnObj(req, etag, msg);

}

status_t Server::returnObject(const req_t& req, shared_ptr<ETagHandler> etag, const DictO &obj, const string &field) {

  // some etags don't use the result so they just always return false.
  if (etag->resultModified(obj, field)) {
    return not_modified(req, etag->origEtag());
  }

  auto result = Dict::getObject(obj, field);
  
  if (!result) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << field << " missing";
    return init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

//  BOOST_LOG_TRIVIAL(debug) << result.value();

  return returnObj(req, etag, Json::fixObject(result.value()));

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

  auto j = callNodes(dictO({
    { "type", "login" },
    { "session", "1" },
    { "password", password }
  }));
  
  auto type = Dict::getString(j, "type");
  if (!type) {
    BOOST_LOG_TRIVIAL(error) << "missing type in return";
    return nullopt;
  }
  
  if (type.value() == "err") {
    BOOST_LOG_TRIVIAL(error) << Dict::getString(j, "msg").value();
    return nullopt;
  }

  return Sessions::instance()->create(j);
  
}
