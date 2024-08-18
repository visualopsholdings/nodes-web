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

namespace nodes {

status_t getinfos(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }
  server->send({ 
    { "type", "infos" }
  });
  json j = server->receive();
  auto infos = Json::getArray(j, "infos");
  
  if (!infos) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "infos missing infos";
    return server->init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

  auto resp = server->init_resp( req->create_response() );

  stringstream ss;
  ss << infos.value();
  resp.set_body(ss.str());

  return resp.done();
}

};
