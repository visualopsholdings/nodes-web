/*
  getsites.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 18-Sep-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <restinio/core.hpp>

namespace nodes {

status_t getsites(Server *server, const req_t& req, params_t params)
{

  auto etag = ETag::none(req);
  if (!etag) {
    return server->not_modified(req);
  }

	// for now we have no restrictions but if more things get added to a site
	// we need to filter for just what ANY user should get.
  server->send({ { "type", "site" } });
  return server->receiveObject(req, etag, "site");

}

};


