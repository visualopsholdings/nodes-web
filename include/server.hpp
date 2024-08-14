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
};

using status_t = restinio::request_handling_status_t;
using req_t = restinio::request_handle_t;
using params_t = restinio::router::route_params_t;

class Session;

class Server {

public:
  Server(const string &reqConn);
    
  void run(int httpPort);
  auto handler();
  void send(const json &json);
  json receive();
  
	status_t getroot(const req_t& req, params_t );
	status_t getfonts(const req_t& req, params_t );
	status_t getrawusers(const req_t& req, params_t );
	status_t getstreams(const req_t& req, params_t );
	status_t getstream(const req_t& req, params_t );
	status_t getconversation(const req_t& req, params_t );
	status_t getstreampolicyusers(const req_t& req, params_t );
	status_t getme(const req_t& req, params_t );
	status_t getlogin(const req_t& req, params_t );
	status_t postlogin(const req_t& req, params_t );
	status_t getlogout(const req_t& req, params_t );
	status_t getuser(const req_t& req, params_t );
	status_t postideas(const req_t& req, params_t );
	status_t posttyping(const req_t& req, params_t );
	status_t getinfos(const req_t& req, params_t );
	status_t postinfos(const req_t& req, params_t );
	status_t getsite(const req_t& req, params_t );
	status_t putsite(const req_t& req, params_t );

private:
  zmq::context_t _context;
  zmq::socket_t _req;

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

  optional<string> finishlogin(const string &password);
  status_t unauthorised(const req_t& req);
  status_t fatal(const req_t& req, const string &msg);
  optional<shared_ptr<Session> > getSession(const req_t& req);
  bool isAdmin(const req_t& req);
  status_t sendBodyReturnEmptyObj(const req_t& req, const string &type);

};

#endif // H_server
