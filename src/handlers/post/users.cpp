/*
  postusers.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 15-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

status_t Server::postusers(const req_t& req, params_t params) {

  if (!isAdmin(req)) {
    return unauthorised(req);
  }
  
  json j = boost::json::parse(req->body());
  BOOST_LOG_TRIVIAL(trace) << j;

  auto query = Json::getBool(j, "query");
  if (!query || !query.value()) {
    return fatal(req, "only understand query.");
  }
  
  auto email = Json::getString(j, "email");
  if (!email) {
    return fatal(req, "query requires email.");
  }
  
  j = {
    { "type", "query" },
    { "objtype", "user" },      
    { "email", email.value() }
  };
	send(j);
  j = receive();
  
  BOOST_LOG_TRIVIAL(trace) << j;
  
  return returnEmptyObj(req);

}
