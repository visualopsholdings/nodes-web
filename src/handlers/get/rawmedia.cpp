/*
  getrawmedia.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 27-Nov-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <restinio/core.hpp>

namespace nodes {

status_t getrawmedia(Server *server, const req_t& req, params_t params)
{
  if (!server->isAdmin(req)) {
    return server->unauthorised(req);
  }
  
  auto msg = dictO({
    { "type", "objects" },
    { "objtype", "media" }
  });
  auto etag = ETag::collectionChanged(req, &msg);
  auto j = server->callNodes(msg);

  auto resp = server->checkErrors(req, j, "media");
  if (resp) {
    return resp.value();
  }
  return server->returnArray(req, etag, j, "media");

}

};
