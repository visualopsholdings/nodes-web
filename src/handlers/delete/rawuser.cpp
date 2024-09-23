/*
  deleterawuser.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 23-Sep-2024
    
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

status_t deleterawuser(Server *server, const req_t& req, params_t params) {

  if (!server->isAdmin(req)) {
    return server->unauthorised(req);
  }
  
  const auto id = restinio::cast_to<string>(params["id"]);
  
	server->send({
	  { "type", "deleteuser" },
	  { "id",  id }
	});
  json j = server->receive();
  
//  BOOST_LOG_TRIVIAL(trace) << j;
  
  return server->checkErrorsReturnEmptyObj(req, j, "deleteuser");

}

};
