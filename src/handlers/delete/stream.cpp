/*
  deletestream.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 10-Sep-2024
    
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

status_t deletestream(Server *server, const req_t& req, params_t params) {

  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }

  const auto id = restinio::cast_to<string>(params["id"]);
  
  auto j = server->callNodes(dictO({
    { "type", "deleteobject" },
    { "objtype", "stream" },
	  { "id",  id },
    { "me", session.value()->userid() }
	}));
  
  auto resp = server->checkErrors(req, j, "deletestream");
  if (resp) {
    return resp.value();
  }
  
  return server->returnEmptyObj(req, ETag::none());

}

};
