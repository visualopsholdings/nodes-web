/*
  postlogin.cpp
  
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

namespace nodes {

status_t postlogin(Server *server, const req_t& req, params_t params) {

  json j = boost::json::parse(req->body());
  BOOST_LOG_TRIVIAL(trace) << "postlogin " << j;

  auto password = Json::getString(j, "password");
  if (!password) {
    BOOST_LOG_TRIVIAL(error) << "missing password";
    return server->unauthorised(req);
  }

  auto id = server->finishlogin(password.value());
  if (!id) {
    return server->unauthorised(req);
  }

  auto resp = req->create_response();
	resp.append_header("Set-Cookie", "ss-id=" + id.value() + "; Path=/; Secure; HttpOnly");
  resp.append_header("Cache-Control", "nocache");
  resp.set_body("{}");
  return resp.done();

}

};
