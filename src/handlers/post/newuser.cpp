/*
  postusers.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 15-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "etag.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

namespace nodes {

status_t postnewuser(Server *server, const req_t& req, params_t params) {

  auto etag = ETag::none();

  auto j = Dict::getObject(Dict::parseString(req->body()));
  if (!j) {
    return server->fatal(req, "could not parse body to JSON.");
  }
  BOOST_LOG_TRIVIAL(trace) << Dict::toString(*j);

  auto fullname = Dict::getString(j, "fullname");
  auto vopsidtoken = Dict::getString(j, "vopsidtoken");
  if (!fullname || !vopsidtoken) {
    // don't let on.
    return server->returnEmptyObj(req, etag);
  }
  
  j = server->callNodes(dictO({
      { "type", "adduser" },
      { "vopsidtoken", vopsidtoken.value() },
      { "fullname", fullname.value() },
      { "collection", "streams" },
  }));

  auto resp1 = server->checkErrors(req, *j, "postnewuser");
  if (resp1) {
    return resp1.value();
  }
  
  auto id = Dict::getString(j, "id");
  fullname = Dict::getString(j, "fullname");
  if (!id && !fullname) {
    return server->fatal(req, "no id and fullname in response");
  }
  
  // create a login session.
  string session = Sessions::instance()->create(dictO({
    { "id", id.value() },
    { "fullname", fullname.value() },
  }));
  
  // return what we got back from nodes and set the session if.
  auto resp = req->create_response();
  resp.set_body(Dict::toString(*j, false));
  resp.append_header("Set-Cookie", "ss-id=" + session + "; Path=/; Secure; HttpOnly");
  etag->setHeaders(resp);
  return resp.done();

}

};
