/*
  postusers.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 15-Aug-2024
    
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

status_t postnewuser(Server *server, const req_t& req, params_t params) {

  json j = boost::json::parse(req->body());
  BOOST_LOG_TRIVIAL(trace) << j;

  auto fullname = Json::getString(j, "fullname");
  auto vopsidtoken = Json::getString(j, "vopsidtoken");
  if (!fullname || !vopsidtoken) {
    // don't let on.
    return server->returnEmptyObj(req);
  }
  
  server->send({
      { "type", "newuser" },
      { "vopsidtoken", vopsidtoken.value() },
      { "fullname", fullname.value() }
  });
  return server->receiveRawObject(req);

}

};
