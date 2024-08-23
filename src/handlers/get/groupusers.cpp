/*
  getgroupusers.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 23-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

namespace nodes {

status_t getgroupusers(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }
  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyObj(req);
  }
  server->send({ 
    { "type", "members" },
    { "group", id }
  });
  json j = server->receive();
  auto members = Json::getArray(j, "members");
  if (!members) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "members missing members";
    return server->init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

  auto resp = server->init_resp( req->create_response() );

  boost::json::array newmembers;
  for (auto s: members.value()) {
    s.as_object()["_id"] = Json::getString(s, "user").value();
    newmembers.push_back(s);
  }
  stringstream ss;
  ss << newmembers;
  resp.set_body(ss.str());

  return resp.done();
}

};
