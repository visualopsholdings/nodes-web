/*
  posttyping.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 30-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>

namespace nodes {

status_t posttyping(Server *server, const req_t& req, params_t params) {

  return server->returnEmptyObj(req, ETag::none());

}

};
