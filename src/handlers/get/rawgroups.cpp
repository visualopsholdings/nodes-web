/*
  getrawgroups.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 23-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>

namespace nodes {

status_t getrawgroups(Server *server, const req_t& req, params_t params)
{
  if (!server->isAdmin(req)) {
    return server->unauthorised(req);
  }
  
  server->send({ { "type", "groups" } });
  return server->receiveArray(req, "groups");

}

};
