/*
  getstreampolicyteams.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 12-Sep-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <restinio/core.hpp>

namespace nodes {

status_t getstreampolicygroups(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }
  auto etag = ETag::none(req);
  if (!etag) {
    return server->not_modified(req);
  }

  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    auto resp = server->init_resp( req->create_response() );
    resp.set_body("[]");
    return resp.done();
  }
  server->send({ 
    { "type", "stream" },
    { "stream", id }
  });
  json j = server->receive();
  auto stream = Json::getObject(j, "stream");
  if (!stream) {
    return server->fatal(req, "stream missing stream");
  }
  auto policy = Json::getString(stream.value(), "policy");
  if (!policy) {
    return server->fatal(req, "stream missing policy");
  }
  server->send({ 
    { "type", "policygroups" },
    { "policy", policy.value() }
  });
  return server->receiveArray(req, etag, "groups");

}

};

