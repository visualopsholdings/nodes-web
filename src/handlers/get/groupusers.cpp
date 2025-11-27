/*
  getgroupusers.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 27-Aug-2024
    
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

status_t getgroupusers(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }
  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyObj(req, ETag::none());
  }
  auto msg = dictO({
    { "type", "members" },
    { "me", session.value()->userid() },
    { "group", id }
  });
  auto etag = ETag::modifyDate(req, &msg);
  auto j = server->callNodes(msg);

  auto resp = server->checkErrors(req, j, "group");
  if (resp) {
    return resp.value();
  }
  
  return server->returnObject(req, etag, j, "group");

}

};
