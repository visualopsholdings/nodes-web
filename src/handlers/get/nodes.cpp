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
  
  auto msg = dictO({
    { "type", "nodes" }
  });
  auto etag = ETag::collectionChanged(req, &msg);
  auto j = server->callNodes(msg);

  auto resp = server->checkErrors(req, j, "nodes");
  if (resp) {
    return resp.value();
  }
  return server->returnArray(req, etag, j, "nodes");

}

};
