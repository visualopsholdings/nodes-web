/*
  getinfos.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 7-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

status_t Server::getinfos(
  const req_t& req, params_t params)
{
  auto session = getSession(req);
  if (!session) {
    return unauthorised(req);
  }
  send({ 
    { "type", "infos" }
  });
  json j = receive();
  auto infos = Json::getArray(j, "infos");
  
  if (!infos) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "infos missing infos";
    return init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

  auto resp = init_resp( req->create_response() );

  stringstream ss;
  ss << infos.value();
  resp.set_body(ss.str());

  return resp.done();
}
