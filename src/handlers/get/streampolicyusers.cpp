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

status_t Server::getstreampolicyusers(
  const req_t& req, params_t params)
{
  auto session = getSession(req);
  if (!session) {
    return unauthorised(req);
  }
  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    auto resp = init_resp( req->create_response() );
    resp.set_body("[]");
    return resp.done();
  }
  send({ 
    { "type", "stream" },
    { "stream", id }
  });
  json j = receive();
  auto stream = Json::getObject(j, "stream");
  if (!stream) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "stream missing stream";
    return init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }
  auto policy = Json::getString(stream.value(), "policy");
  if (!policy) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "stream missing policy";
    return init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }
  send({ 
    { "type", "policyusers" },
    { "policy", policy.value() }
  });
  j = receive();
  auto users = Json::getArray(j, "policyusers");
  if (!users) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "policyusers missing users";
    return init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

  boost::json::array newusers;
  for (auto s: users.value()) {
    s.as_object()["_id"] = Json::getString(s, "id").value();
    newusers.push_back(s);
  }

  auto resp = init_resp( req->create_response() );
  stringstream ss;
  ss << newusers;
  resp.set_body(ss.str());

  return resp.done();
}

