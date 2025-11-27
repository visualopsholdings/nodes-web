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
  
  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyObj(req, ETag::none());
  }
  auto msg = dictO({
    { "type", "user" },
    { "user", id }
  });
  auto etag = ETag::modifyDate(req, &msg);
  auto j = server->callNodes(msg);

  auto resp = server->checkErrors(req, j, "user");
  if (resp) {
    return resp.value();
  }
  
  if (etag->resultModified(j, "user")) {
    return server->not_modified(req, etag->origEtag());
  }

  auto user = Dict::getObject(j, "user");
  if (!user) {
    // send fatal error
    return server->fatal(req, "user missing user");
  }

  // return a subset of the user.
  auto newuser = dictO({
    { "_id", Dict::getString(user.value(), "id").value() },
    { "fullname", Dict::getString(user.value(), "fullname").value() },
    { "modifyDate", Dict::getString(user.value(), "modifyDate").value() }
  });
  
  return server->returnObj(req, etag, newuser);

}

};


