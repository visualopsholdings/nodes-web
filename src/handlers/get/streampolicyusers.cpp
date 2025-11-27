/*
  getstreampolicyusers.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 26-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <restinio/core.hpp>

namespace nodes {

status_t getstreampolicyusers(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }

  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyArray(req, ETag::none());
  }
  auto msg = dictO({
    { "type", "object" },
    { "objtype", "stream" },
    { "id", id }
  });
  auto etag = ETag::modifyDate(req, &msg);
  auto j = server->callNodes(msg);
  
  auto resp = server->checkErrors(req, j, "getstreams");
  if (resp) {
    return resp.value();
  }
  
  if (etag->resultModified(j, "stream")) {
    return server->not_modified(req, etag->origEtag());
  }
  auto stream = Dict::getObject(j, "stream");
  if (!stream) {
    return server->fatal(req, "stream missing stream");
  }
  auto policy = Dict::getString(stream.value(), "policy");
  if (!policy) {
    return server->fatal(req, "stream missing policy");
  }
  j = server->callNodes(dictO({ 
    { "type", "policyusers" },
    { "policy", policy.value() }
  }));

  resp = server->checkErrors(req, j, "users");
  if (resp) {
    return resp.value();
  }
  return server->returnArray(req, etag, j, "users");

}

};

