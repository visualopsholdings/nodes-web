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
  auto etag = ETag::none(req);
  if (!etag) {
    return server->not_modified(req);
  }

  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyObj(req, etag);
  }
  server->send({ 
    { "type", "members" },
    { "me", session.value()->userid() },
    { "group", id }
  });
  json j = server->receive();
  auto members = Json::getArray(j, "members");
  if (!members) {
    return server->fatal(req, "members missing members");
  }

  auto resp = server->init_resp( req->create_response() );

  boost::json::array newmembers;
  for (auto s: members.value()) {
    s.as_object()["_id"] = Json::getString(s, "user").value();
    newmembers.push_back(s);
  }
  stringstream ss;
  ss << newmembers;
  resp.set_body(ss.str());
  etag->setHeaders(resp);
  
  return resp.done();
}

};
