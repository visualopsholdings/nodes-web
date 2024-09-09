/*
  getrawstream.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 9-Sep-2024
    
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

status_t getrawstreampolicy(Server *server, const req_t& req, params_t params)
{
  if (!server->isAdmin(req)) {
    return server->unauthorised(req);
  }
  
  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyObj(req);
  }
  server->send({ 
    { "type", "policy" },
    { "objtype", "stream" },
    { "id", id }
  });
  json j = server->receive();
  auto policy = Json::getArray(j, "policy");
  if (!policy) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "policy missing policy";
    return server->init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

  auto resp = server->init_resp( req->create_response() );

  stringstream ss;
  ss << policy.value();
  resp.set_body(ss.str());

  return resp.done();
}

};


