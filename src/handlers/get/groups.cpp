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

  auto msg = dictO({
    { "type", "groups" },
    { "me", session.value()->userid() }
  });
  auto etag = ETag::collectionChanged(req, &msg);
  auto j = server->callNodes(msg);

  auto resp = server->checkErrors(req, j, "groups");
  if (resp) {
    return resp.value();
  }
  return server->returnArray(req, etag, j, "groups");

}

};
