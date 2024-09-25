/*
  getuser.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 26-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <restinio/core.hpp>

namespace nodes {

status_t getuser(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }
  auto etag = ETag::simpleTime(req, session.value());
  if (!etag) {
    return server->not_modified(req);
  }
  
  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyObj(req, etag);
  }
  server->send({ 
    { "type", "user" },
    { "user", id }
  });
  
  json j = server->receive();
  auto user = Json::getObject(j, "user");
  if (!user) {
    // send fatal error
    return server->fatal(req, "user missing user");
  }

  // return a subset of the user.
  json newuser = {
    { "_id", Json::getString(user.value(), "id").value() },
    { "fullname", Json::getString(user.value(), "fullname").value() }
  };
  
  return server->returnObj(req, etag, newuser);

}

};


