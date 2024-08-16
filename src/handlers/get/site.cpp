/*
  getsite.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 14-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

status_t Server::getsite(
  const req_t& req, params_t params)
{
  if (!isAdmin(req)) {
    return unauthorised(req);
  }
  
  send({ { "type", "site" } });
  json j = receive();
  auto site = Json::getObject(j, "site");
  
  if (!site) {
    BOOST_LOG_TRIVIAL(trace) << "no site";
    return returnEmptyObj(req);
  }

  json newsite = site.value();
  newsite.as_object()["_id"] = Json::getString(newsite, "id").value();

  auto resp = init_resp( req->create_response() );

  stringstream ss;
  ss << newsite;
  resp.set_body(ss.str());

  return resp.done();
}


