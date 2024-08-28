/*
  getrawgroup.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 23-Aug-2024
    
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

status_t getrawgroup(Server *server, const req_t& req, params_t params)
{
  if (!server->isAdmin(req)) {
    return server->unauthorised(req);
  }
  
  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyObj(req);
  }
  server->send({ 
    { "type", "group" },
    { "group", id }
  });
  json j = server->receive();
  auto group = Json::getObject(j, "group");
  if (!group) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "group missing group";
    return server->init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

  json newgroup = group.value();
  newgroup.as_object()["_id"] = Json::getString(newgroup, "id").value();

  auto resp = server->init_resp( req->create_response() );

  stringstream ss;
  ss << newgroup;
  resp.set_body(ss.str());

  return resp.done();
}

};


