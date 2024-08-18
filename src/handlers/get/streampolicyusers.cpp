/*
  getstreampolicyusers.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 26-Jul-2024
    
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

status_t getstreampolicyusers(Server *server, const req_t& req, params_t params)
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
  server->send({ 
    { "type", "stream" },
    { "stream", id }
  });
  json j = server->receive();
  auto stream = Json::getObject(j, "stream");
  if (!stream) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "stream missing stream";
    return server->init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }
  auto policy = Json::getString(stream.value(), "policy");
  if (!policy) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "stream missing policy";
    return server->init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }
  server->send({ 
    { "type", "policyusers" },
    { "policy", policy.value() }
  });
  j = server->receive();
  auto users = Json::getArray(j, "users");
  if (!users) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "policyusers missing users";
    return server->init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

  boost::json::array newusers;
  for (auto s: users.value()) {
    s.as_object()["_id"] = Json::getString(s, "id").value();
    newusers.push_back(s);
  }

  auto resp = server->init_resp( req->create_response() );
  stringstream ss;
  ss << newusers;
  resp.set_body(ss.str());

  return resp.done();
}

};

