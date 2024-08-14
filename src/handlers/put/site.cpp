/*
  putsite.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 14-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>

status_t Server::putsite(const req_t& req, params_t ) {

  if (!isAdmin(req)) {
    return unauthorised(req);
  }
  
  return sendBodyReturnEmptyObj(req, "setsite");
  
}
