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
#include "etag.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

namespace nodes {

status_t postnewuser(Server *server, const req_t& req, params_t params) {

  auto etag = ETag::none();

  json j = boost::json::parse(req->body());
  BOOST_LOG_TRIVIAL(trace) << j;

  auto fullname = Json::getString(j, "fullname");
  auto vopsidtoken = Json::getString(j, "vopsidtoken");
  if (!fullname || !vopsidtoken) {
    // don't let on.
    return server->returnEmptyObj(req, etag);
  }
  
  server->send({
      { "type", "adduser" },
      { "vopsidtoken", vopsidtoken.value() },
      { "fullname", fullname.value() }
  });
  
  j = server->receive();

  auto resp1 = server->checkErrors(req, j, "postnewuser");
  if (resp1) {
    return resp1.value();
  }
  
  json user;
  auto id = Json::getString(j, "id");
  fullname = Json::getString(j, "fullname");
  if (!id && !fullname) {
    return server->fatal(req, "no id and fullname in response");
  }
  
  // create a login session.
  string session = Sessions::instance()->create({
    { "id", id.value() },
    { "fullname", fullname.value() },
  });
  
  // return what we got back from nodes and set the session if.
  auto resp = req->create_response();
  stringstream ss;
  ss << j;
  resp.set_body(ss.str());
  resp.append_header("Set-Cookie", "ss-id=" + session + "; Path=/; Secure; HttpOnly");
  etag->setHeaders(resp);
  return resp.done();

}

};
