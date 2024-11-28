/*
  getstreamsharelink.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 12-Sep-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <restinio/core.hpp>

namespace nodes {

status_t getstreamsharelink(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }
  auto etag = ETag::simpleTime(req, session.value());
  if (!etag->modified()) {
    return server->not_modified(req, etag->origEtag());
  }
  
  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyArray(req, etag);
  }

  const auto qp = restinio::parse_query(req->header().query());
  if (!qp.has("group") || !qp.has("expires")) {
    return server->warning(req, "missing group and expires");
  }
  
  const auto group = restinio::cast_to<string>(qp["group"]);
  const auto expires = restinio::cast_to<int>(qp["expires"]);
  
  server->send({ 
    { "type", "sharelink" },
    { "collection", "streams" },
    { "id", id },
    { "bitsfield", "streambits" },
    { "group", group },
    { "expires", expires },
    { "urlpostfix", "/apps/chat/#/streams/" + id },
    { "me", session.value()->userid() }
  });
  return server->receiveRawObject(req, etag);

}

};

