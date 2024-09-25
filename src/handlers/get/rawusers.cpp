/*
  getrawusers.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 26-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <restinio/core.hpp>

namespace nodes {

status_t getrawusers(Server *server, const req_t& req, params_t params)
{
  if (!server->isAdmin(req)) {
    return server->unauthorised(req);
  }
  
  auto etag = ETag::none(req);
  if (!etag) {
    return server->not_modified(req);
  }
  
  server->send({ { "type", "users" } });
  return server->receiveArray(req, etag, "users");
  
}

};
