/*
  postideas.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Nov-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "etag.hpp"
#include "dict.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

namespace nodes {

status_t postideas(Server *server, const req_t& req, params_t params) {

    auto session = server->getSession(req);
    if (!session) {
      return server->unauthorised(req);
    }
  
    auto body = Dict::getObject(Dict::parseString(req->body()));
    if (!body) {
      return server->fatal(req, "could not parse body to JSON.");
    }
  //  BOOST_LOG_TRIVIAL(trace) << Dict::toString(*body);
  
    (*body)["me"] = session.value()->userid();
  
    auto msg = dictO({
      { "type", "addobject" },
      { "objtype", "idea" }
    });
    
    auto j = server->callNodes(server->mergeBody(req, *body, msg, nullopt));
  
    auto resp = server->checkErrors(req, j, "postideas");
    if (resp) {
      return resp.value();
    }
  
    auto ret = DictO();
    auto result = Dict::getObject(j, "result");
    if (result) {
      BOOST_LOG_TRIVIAL(trace) << Dict::toString(*result);
      auto id = Dict::getString(*result, "id");
      auto mod = Dict::getString(*result, "modifyDate");
      if (id) {
        ret["_id"] = *id;
      }
      if (mod) {
        ret["modifyDate"] = *mod;
      }
    }
    
    return server->returnObj(req, ETag::none(), ret);

}

};
