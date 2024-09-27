/*
  getrawstreams.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 9-Sep-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <restinio/core.hpp>

namespace nodes {

status_t getrawstreams(Server *server, const req_t& req, params_t params)
{
  if (!server->isAdmin(req)) {
    return server->unauthorised(req);
  }
  
  json msg = { 
    { "type", "streams" }
  };
  auto etag = ETag::collectionChanged(req, &msg);
  server->send(msg);
  return server->receiveArray(req, etag, "streams");

}

};
