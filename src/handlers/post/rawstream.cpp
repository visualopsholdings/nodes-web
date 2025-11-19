/*
  postrawstream.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 20-Nov-2024
    
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

status_t postrawstream(Server *server, const req_t& req, params_t params) {

  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyObj(req, ETag::none());
  }

  auto j = Dict::getObject(Dict::parseString(req->body()));
  if (!j) {
    return server->fatal(req, "could not parse body to JSON.");
  }
  BOOST_LOG_TRIVIAL(trace) << Dict::toString(*j);

  // purge
  auto purge = Dict::getBool(j, "purge");
  if (purge && purge.value()) {
    auto msg = dictO({
      { "type", "purge" },
      { "objtype", "idea" },
      { "stream", id }
    });
    return server->sendSimpleReturnEmptyObjAdmin(msg, req);
  }

  return server->fatal(req, "only understand purge and deleteAll.");  

}

};
