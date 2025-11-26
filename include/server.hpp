/*
  server.hpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 26-Jul-2024
    
  A HTTP Server for nodes.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#ifndef H_server
#define H_server

#include "dict.hpp"

#include <string>
#include <map>
#include <zmq.hpp>
#include <restinio/http_headers.hpp>
#include <restinio/settings.hpp>
#include <restinio/router/express.hpp>

using namespace std;
using namespace vops;

namespace restinio {
  namespace router {
    class route_params_t;
  }
  namespace websocket {
    namespace basic {
      class ws_t;
    }
  }
};
namespace rws = restinio::websocket::basic;

using status_t = restinio::request_handling_status_t;
using req_t = restinio::request_handle_t;
using params_t = restinio::router::route_params_t;

class Session;
class ZMQClient;
class ETagHandler;

class Server {

public:
  Server(int reqPort, int subPort, const string &mediaPath, bool test);
    
  void run(int httpPort);
    // run the server.
  
  void send(const DictO &obj);
    // send the object to nodes.
    
  DictO receive();
    // recieve an object back from nodes.
    
  status_t returnObj(const req_t& req, shared_ptr<ETagHandler> etag, const DictG &obj);
    // return an object to the browser.
    
  status_t returnEmptyObj(const req_t& req, shared_ptr<ETagHandler> etag);
    // return an empty object back to the browser.
    
  status_t returnEmptyArray(const req_t& req, shared_ptr<ETagHandler> etag);
    // return an empty array back to the browser.
    
  optional<string> finishlogin(const string &password);
    // finish the login and create a session.
    
  status_t unauthorised(const req_t& req);
  status_t fatal(const req_t& req, const string &msg);
  status_t warning(const req_t& req, const string &msg);
  status_t security(const req_t& req);
  status_t not_modified(const req_t& req, optional<string> etag);
    // warnings/errors back to the browser.
  
  optional<shared_ptr<Session> > getSession(const req_t& req);
    // get our session from the request.
    
  bool isAdmin(const req_t& req);
    // are we admin?
    
  optional<status_t> checkErrors(const req_t& req, const DictO &obj, const string &type);
    // check errors back from nodes.
    
  status_t receiveArray(const req_t& req, shared_ptr<ETagHandler> etag, const string &field);
    // receive a message back from nodes, and pass the named field as an array back to the browser.
    
  status_t receiveObject(const req_t& req, shared_ptr<ETagHandler> etag, const string &field);
    // receive a message back from nodes, and pass the named field as an object back to the browser.
    
  status_t receiveRawObject(const req_t& req, shared_ptr<ETagHandler> etag);
    // receive a message back from nodes, and pass the it straight back to the browser.
    
  DictO mergeBody(const req_t& req, const DictO &body, const DictO &msg, optional<string> id);
    // merge the message fields and the body, the id and a possible corr field into one object.
    
  status_t sendObjReturnEmptyObj(const req_t& req, const DictO &msg, optional<string> id=nullopt);
  status_t sendObjReturnEmptyObjAdmin(const req_t& req, const DictO &msg, optional<string> id=nullopt);
  status_t sendObjReturnRawObj(const req_t& req, const DictO &msg, optional<string> id=nullopt);
    // send the body along with the obj data and return to the brower.
    
  status_t checkErrorsReturnEmptyObj(const req_t& req, const DictO &msg, const string &type);
  
  status_t sendSimpleReturnEmptyObjAdmin(const DictO &msg, const req_t& req);
  status_t sendSimpleReturnRawObjectAdmin(const DictO &msg, const req_t& req);

  void sendWS(uint64_t &id, const DictO &json);
  void sendAllWS(const DictO &json);
  void sendAllWSExcept(const DictO &json, const string &socketid);
  shared_ptr<rws::ws_t> createWS(const req_t& req);
  
	template < typename RESP >
	static RESP
	init_resp( RESP resp )
	{
		resp
			.append_header("Server", "Nodes Server /v.0.1")
			.append_header_date_field()
			.append_header("Content-Type", "application/json; charset=utf-8");

		return resp;
	}

  bool _test;
  string _mediaPath;
  
private:
  zmq::context_t _context;
  zmq::socket_t _req;
  shared_ptr<ZMQClient> _zmq;
  map<uint64_t, std::shared_ptr<rws::ws_t> > _registry;
    
  std::unique_ptr<restinio::router::express_router_t<> > createRouter();
    // register all of the message handlers and return the router object.
};

#endif // H_server
