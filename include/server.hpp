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

#include <string>
#include <map>
#include <boost/json.hpp>
#include <zmq.hpp>
#include <restinio/http_headers.hpp>
#include <restinio/settings.hpp>

using namespace std;
using json = boost::json::value;
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
  void send(const json &json);
  json receive();
  optional<string> finishlogin(const string &password);
  status_t unauthorised(const req_t& req);
  status_t fatal(const req_t& req, const string &msg);
  status_t warning(const req_t& req, const string &msg);
  status_t security(const req_t& req);
  status_t not_modified(const req_t& req, optional<string> etag);
  optional<shared_ptr<Session> > getSession(const req_t& req);
  bool isAdmin(const req_t& req);
  optional<status_t> checkErrors(const req_t& req, json &j, const string &type);
  
  status_t returnObj(const req_t& req, shared_ptr<ETagHandler> etag, json &j);
  status_t receiveArray(const req_t& req, shared_ptr<ETagHandler> etag, const string &field);
  status_t receiveObject(const req_t& req, shared_ptr<ETagHandler> etag, const string &field);
  status_t receiveRawObject(const req_t& req, shared_ptr<ETagHandler> etag);
  status_t returnEmptyObj(const req_t& req, shared_ptr<ETagHandler> etag);
  status_t returnEmptyArray(const req_t& req, shared_ptr<ETagHandler> etag);

  status_t sendBodyReturnEmptyObj(const req_t& req, json &msg, optional<string> id=nullopt);
  status_t sendBodyReturnEmptyObjAdmin(const req_t& req, json &msg, optional<string> id=nullopt);
  status_t checkErrorsReturnEmptyObj(const req_t& req, json &msg, const string &type);
  status_t sendSimpleReturnEmptyObjAdmin(json &msg, const req_t& req);
  status_t sendSimpleReturnRawObjectAdmin(json &msg, const req_t& req);

  void sendWS(uint64_t &id, const json &json);
  void sendAllWS(const json &json);
  void sendAllWSExcept(const json &json, const string &socketid);
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
  
  status_t sendBody(const req_t& req, shared_ptr<ETagHandler> etag, json &body, json &msg, optional<string> id);
  
};

#endif // H_server
