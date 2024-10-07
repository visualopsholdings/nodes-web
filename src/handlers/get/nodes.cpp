/*
  getnodes.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 7-Oct-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <restinio/core.hpp>

namespace nodes {

status_t getnodes(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }
  if (!session.value()->isAdmin()) {
    return server->unauthorised(req);
  }
  
  json msg = { 
    { "type", "nodes" }
  };
  auto etag = ETag::collectionChanged(req, &msg);
  server->send(msg);
  return server->receiveArray(req, etag, "nodes");

}

};
