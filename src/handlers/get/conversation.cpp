/*
  getconversation.cpp
  
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

status_t getconversation(Server *server, const req_t& req, params_t params)
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
    { "type", "objects" },
    { "objtype", "idea" },
    { "stream", id },
    { "me", session.value()->userid() }
  });
  auto etag = ETag::collectionChanged(req, &msg);
  auto j = server->callNodes(msg);

  auto resp = server->checkErrors(req, j, "ideas");
  if (resp) {
    return resp.value();
  }

  return server->returnArray(req, etag, j, "ideas");

}

};
