/*
  postrawusers.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 19-Nov-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

namespace nodes {

status_t postrawusers(Server *server, const req_t& req, params_t params) {

  json j = boost::json::parse(req->body());
  BOOST_LOG_TRIVIAL(trace) << j;

  // purge
  auto purge = Json::getBool(j, "purge", true);
  if (purge && purge.value()) {
    return server->sendSimpleReturnEmptyObjAdmin("purgeusers", req);
  }

  return server->fatal(req, "only understand purge.");  

}

};
