/*
  getlogin.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 26-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

namespace nodes {

status_t getlogin(Server *server, const req_t& req, params_t params)
{
  const auto qp = restinio::parse_query(req->header().query());
  if (qp.has("username")) {
    const auto username = restinio::cast_to<string>(qp["username"]);
    auto id = server->finishlogin(username);
    if (!id) {
      BOOST_LOG_TRIVIAL(trace) << "no id generated";  
      return server->unauthorised(req);
    }
    BOOST_LOG_TRIVIAL(trace) << "sending response with cookie " << id.value();  
    auto resp = req->create_response(restinio::status_found());
    string app = "chat";
    if (qp.has("app")) {
      app = restinio::cast_to<string>(qp["app"]);
    }
	  resp.append_header("Location", "/apps/" + app + "/#/");
	  string cookie = "ss-id=" + id.value() + "; Path=/; HttpOnly";
	  if (!server->_test) {
	    cookie += "; Secure";
	  }
	  resp.append_header("Set-Cookie", cookie);
	  resp.append_header("Cache-Control", "nocache");
    return resp.done();
  }

  auto resp = req->create_response(restinio::status_found());
	resp.append_header("Location", "/#/");
  return resp.done();
}

};
