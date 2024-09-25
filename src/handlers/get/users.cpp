/*
  getusers.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 23-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <restinio/core.hpp>

namespace nodes {

status_t getusers(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }
  auto etag = ETag::none();

  const auto qp = restinio::parse_query(req->header().query());
  if (qp.has("q")) {
    const auto q = restinio::cast_to<string>(qp["q"]);
    server->send({ 
      { "type", "searchusers" },
      { "q", q }
    });
    return server->receiveArray(req, etag, "result");
  }
  
  return server->fatal(req, "only understand query.");

}

};
