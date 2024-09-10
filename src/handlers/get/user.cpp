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

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
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
    return server->returnEmptyObj(req);
  }
  server->send({ 
    { "type", "user" },
    { "user", id }
  });
  
  json j = server->receive();
  auto user = Json::getObject(j, "user");
  if (!user) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "user missing user";
    return server->init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

  // return a subset of the user.
  json newuser = {
    { "_id", Json::getString(user.value(), "id").value() },
    { "fullname", Json::getString(user.value(), "fullname").value() }
  };
  
  auto resp = server->init_resp( req->create_response() );

  stringstream ss;
  ss << newuser;
  resp.set_body(ss.str());

  return resp.done();
}

};


