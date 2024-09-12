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

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

namespace nodes {

status_t getstreamsharelink(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }
  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    auto resp = server->init_resp( req->create_response() );
    resp.set_body("[]");
    return resp.done();
  }

  const auto qp = restinio::parse_query(req->header().query());
  if (!qp.has("group") || !qp.has("expires")) {
    return server->warning(req, "missing group and expires");
  }
  
  const auto group = restinio::cast_to<string>(qp["group"]);
  const auto expires = restinio::cast_to<int>(qp["expires"]);
  
  server->send({ 
    { "type", "streamsharelink" },
    { "stream", id },
    { "group", group },
    { "expires", expires },
    { "me", session.value()->userid() }
  });
  json j = server->receive();

  auto resp = server->checkErrors(req, j, "streamsharelink");
  if (resp) {
    return resp.value();
  }

  return server->returnObj(req, j);

}

};

