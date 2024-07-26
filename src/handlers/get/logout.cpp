/*
  getlogout.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 26-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/czhttp
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"
#include "cookie.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>

status_t Server::getlogout(
  const req_t& req, params_t params)
{
  BOOST_LOG_TRIVIAL(trace) << "GET /logout";
  
  if (!req->header().has_field("Cookie")) {
    BOOST_LOG_TRIVIAL(trace) << "no Cookie";  
    return unauthorised(req);
  }
  auto cookie = req->header().get_field("Cookie");
  auto id = Cookie::parseCookie(cookie);
  if (!id) {
    BOOST_LOG_TRIVIAL(trace) << "couldn't find id in cookie " << cookie;  
    return unauthorised(req);
  }
  Sessions::instance()->destroy(id.value());
  
  auto resp = req->create_response(restinio::status_found());
	resp.append_header("Location", "/apps/login");
  return resp.done();
  
}

