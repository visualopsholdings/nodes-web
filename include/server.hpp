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
  Server(int reqPort, int subPort, bool test);
    
  void run(int httpPort);
  auto handler();
  void send(const DictO &json);
  DictO receive();
  optional<string> finishlogin(const string &password);
  status_t unauthorised(const req_t& req);
  status_t fatal(const req_t& req, const string &msg);
  status_t warning(const req_t& req, const string &msg);
  status_t security(const req_t& req);
  status_t not_modified(const req_t& req, optional<string> etag);
  optional<shared_ptr<Session> > getSession(const req_t& req);
  bool isAdmin(const req_t& req);
  optional<status_t> checkErrors(const req_t& req, const DictO &j, const string &type);
  
  status_t returnObj(const req_t& req, shared_ptr<ETagHandler> etag, const DictG &j);
  status_t receiveArray(const req_t& req, shared_ptr<ETagHandler> etag, const string &field);
  status_t receiveObject(const req_t& req, shared_ptr<ETagHandler> etag, const string &field);
  status_t receiveRawObject(const req_t& req, shared_ptr<ETagHandler> etag);
  status_t returnEmptyObj(const req_t& req, shared_ptr<ETagHandler> etag);
  status_t returnEmptyArray(const req_t& req, shared_ptr<ETagHandler> etag);

  void sendBody(const req_t& req, const DictO &body, const DictO &msg, string *type, optional<string> id);
  status_t sendBodyReturnEmptyObj(const req_t& req, const DictO &msg, optional<string> id=nullopt);
  status_t sendBodyReturnEmptyObjAdmin(const req_t& req, const DictO &msg, optional<string> id=nullopt);
  status_t sendBodyReturnRawObj(const req_t& req, const DictO &msg, optional<string> id=nullopt);
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

private:
  zmq::context_t _context;
  zmq::socket_t _req;
  shared_ptr<ZMQClient> _zmq;
  map<uint64_t, std::shared_ptr<rws::ws_t> > _registry;
    
};

#endif // H_server
