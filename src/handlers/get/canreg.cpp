/*
  getcanreg.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 16-Sep-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <restinio/core.hpp>

namespace nodes {

status_t getcanreg(Server *server, const req_t& req, params_t params)
{
  const auto token = restinio::cast_to<string>(params["token"]);

  auto etag = ETag::none(req);
  if (!etag) {
    return server->not_modified(req);
  }
  
  server->send({ 
    { "type", "canreg" },
    { "token", token }
  });
  return server->receiveRawObject(req, etag);

}

};
