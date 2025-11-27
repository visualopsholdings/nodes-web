/*
  postgroupusers.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 23-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

namespace nodes {

status_t postgroupusers(Server *server, const req_t& req, params_t params) {

  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }

  const auto id = restinio::cast_to<string>(params["id"]);
  
  auto j = Dict::getObject(Dict::parseString(req->body()));
  if (!j) {
    return server->fatal(req, "could not parse body to JSON.");
  }
  BOOST_LOG_TRIVIAL(trace) << Dict::toString(*j);

  auto bodyid = Dict::getString(j, "_id");
  if (!bodyid) {
    return server->fatal(req, "body missing _id");
  }
  
  auto j2 = server->callNodes(dictO({
	  { "type", "addmember" },
	  { "group",  id },
	  { "id",  *bodyid },
    { "me", session.value()->userid() }
	}));

  auto resp = server->checkErrors(req, j2, "addmember");
  if (resp) {
    return resp.value();
  }
  
  return server->returnEmptyObj(req, ETag::none());

}

};
