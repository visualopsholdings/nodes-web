/*
  getstream.cpp
  
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

status_t getstream(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }

  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyObj(req, ETag::none());
  }
  
  json msg = { 
    { "type", "object" },
    { "objtype", "stream" },
    { "me", session.value()->userid() },
    { "id", id }
  };
  auto etag = ETag::modifyDate(req, &msg);
  server->send(msg);
  return server->receiveObject(req, etag, "stream");

}

};

