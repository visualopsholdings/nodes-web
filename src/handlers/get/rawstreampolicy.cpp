/*
  getrawstreampolicy.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 9-Sep-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <restinio/core.hpp>

namespace nodes {

status_t getrawstreampolicy(Server *server, const req_t& req, params_t params)
{
  if (!server->isAdmin(req)) {
    return server->unauthorised(req);
  }
  auto etag = ETag::none();
  
  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyObj(req, etag);
  }
  server->send(dictO({ 
    { "type", "policy" },
    { "objtype", "stream" },
    { "id", id }
  }));
  return server->receiveArray(req, etag, "policy");

}

};


