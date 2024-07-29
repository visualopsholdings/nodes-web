/*
  getfonts.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 26-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"

#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

status_t Server::getfonts(
  const req_t& req, params_t params)
{
  const auto file = restinio::cast_to<string>( params[ "file" ] );
//  BOOST_LOG_TRIVIAL(trace) << "font " << file;

  auto resp = req->create_response();
  resp.set_body(restinio::sendfile("../frontend/fonts/" + file));
  
  return resp.done();
}
