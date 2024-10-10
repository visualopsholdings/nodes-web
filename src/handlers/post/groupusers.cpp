/*
  postgroupusers.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 23-Aug-2024
    
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

status_t postgroupusers(Server *server, const req_t& req, params_t params) {

  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }

  const auto id = restinio::cast_to<string>(params["id"]);
  
  json j = boost::json::parse(req->body());

//  BOOST_LOG_TRIVIAL(trace) << j;

  if (!j.is_object()) {
    return server->fatal(req, "body is not object");
  }
  if (!j.as_object().if_contains("_id")) {
    return server->fatal(req, "body missing _id");
  }
  
	server->send({
	  { "type", "addmember" },
	  { "group",  id },
	  { "id",  j.at("_id").as_string() },
    { "me", session.value()->userid() }
	});
  j = server->receive();
  
//  BOOST_LOG_TRIVIAL(trace) << j;
  
  return server->checkErrorsReturnEmptyObj(req, j, "addmember");

}

};
