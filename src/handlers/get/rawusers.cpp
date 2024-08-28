/*
  getrawusers.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 26-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>

namespace nodes {

status_t getrawusers(Server *server, const req_t& req, params_t params)
{
  if (!server->isAdmin(req)) {
    return server->unauthorised(req);
  }
  
  server->send({ { "type", "users" } });
  json j = server->receive();
  auto users = Json::getArray(j, "users");
  
  if (!users) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "users missing users ";
    return server->init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

  auto resp = server->init_resp( req->create_response() );

  boost::json::array newusers;
  for (auto s: users.value()) {
    s.as_object()["_id"] = Json::getString(s, "id").value();
    newusers.push_back(s);
  }
  stringstream ss;
  ss << newusers;
  resp.set_body(ss.str());

  return resp.done();

}

};
