/*
  postinfos.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 8-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

status_t Server::postinfos(const req_t& req, params_t params) {

  auto session = getSession(req);
  if (!session) {
    return unauthorised(req);
  }
  json j = boost::json::parse(req->body());
  BOOST_LOG_TRIVIAL(trace) << "postinfos " << j;

  auto serverId = Json::getString(j, "serverId");
  if (serverId && serverId.value() == "none") {
    BOOST_LOG_TRIVIAL(trace) << "Resetting server id";
  }
  
  auto resp = req->create_response();
  resp.set_body("{}");
  return resp.done();
  
}
