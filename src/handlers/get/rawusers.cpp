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

status_t Server::getrawusers(
  const req_t& req, params_t )
{
  if (!isAdmin(req)) {
    return unauthorised(req);
  }
  
  send({ { "type", "users" } });
  json j = receive();
  auto users = Json::getArray(j, "users");
  
  if (!users) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "users missing users ";
    return init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

  auto resp = init_resp( req->create_response() );

  stringstream ss;
  ss << users.value();
  resp.set_body(ss.str());

  return resp.done();
}
