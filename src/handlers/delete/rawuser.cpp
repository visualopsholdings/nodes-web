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
#include "etag.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

namespace nodes {

status_t deleterawuser(Server *server, const req_t& req, params_t params) {

  if (!server->isAdmin(req)) {
    return server->unauthorised(req);
  }
  
  const auto id = restinio::cast_to<string>(params["id"]);
  
  auto j = server->callNodes(dictO({
	  { "type", "deleteuser" },
	  { "id",  id }
	}));
  
  auto resp = server->checkErrors(req, j, "deleteuser");
  if (resp) {
    return resp.value();
  }
  
  return server->returnEmptyObj(req, ETag::none());

}

};
