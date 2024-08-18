/*
  getme.cpp
  
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

namespace nodes {

status_t getme(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }
  auto resp = server->init_resp( req->create_response() );

  json j = { 
    { "id", session.value()->userid() },
    { "_id", session.value()->userid() },
    { "name", session.value()->name() },
    { "fullname", session.value()->fullname() },
    { "admin", session.value()->admin() }
  };
  stringstream ss;
  ss << j;
  resp.set_body(ss.str());

  return resp.done();
}

};
