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
#include "etag.hpp"

#include <restinio/core.hpp>

namespace nodes {

status_t getrawgroup(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }
  if (!session.value()->isAdmin()) {
    return server->unauthorised(req);
  }

  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyObj(req, ETag::none());
  }
  json msg = { 
    { "type", "group" },
    { "group", id }
  };
  auto etag = ETag::modifyDate(req, &msg);
  server->send(msg);
  return server->receiveObject(req, etag, "group");

}

};


