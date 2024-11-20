/*
  getideaspurgecount.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 20-Nov-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "json.hpp"
#include "session.hpp"
#include "etag.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

namespace nodes {

status_t getideaspurgecount(Server *server, const req_t& req, params_t params)
{
  if (!server->isAdmin(req)) {
    return server->unauthorised(req);
  }
  auto etag = ETag::none();

  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyObj(req, etag);
  }

  json msg = { 
    { "type", "purgecountideas" },
    { "stream", id }
  };
  server->send(msg);
  return server->receiveRawObject(req, etag);

}

};
