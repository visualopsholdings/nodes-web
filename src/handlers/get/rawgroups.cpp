/*
  getrawgroups.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 23-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>

namespace nodes {

status_t getrawgroups(Server *server, const req_t& req, params_t params)
{
  BOOST_LOG_TRIVIAL(trace) << "getrawgroups";

  if (!server->isAdmin(req)) {
    return server->unauthorised(req);
  }
  
  server->send({ { "type", "groups" } });
  json j = server->receive();
  auto groups = Json::getArray(j, "groups");
  
  if (!groups) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "groups missing groups ";
    return server->init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

  auto resp = server->init_resp( req->create_response() );

  boost::json::array newgroups;
  for (auto s: groups.value()) {
    s.as_object()["_id"] = Json::getString(s, "id").value();
    newgroups.push_back(s);
  }
  stringstream ss;
  ss << newgroups;
  resp.set_body(ss.str());

  return resp.done();
}

};
