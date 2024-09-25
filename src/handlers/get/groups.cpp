/*
  getgroups.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 27-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "json.hpp"
#include "session.hpp"
#include "etag.hpp"

#include <restinio/core.hpp>

namespace nodes {

status_t getgroups(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }
  auto etag = ETag::none(req);
  if (!etag) {
    return server->not_modified(req);
  }

  server->send({ 
    { "type", "groups" },
    { "me", session.value()->userid() }
  });
  return server->receiveArray(req, etag, "groups");

}

};
