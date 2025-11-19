/*
  poststreams.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 20-Oct-2024
    
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

status_t poststreams(Server *server, const req_t& req, params_t params) {

  auto etag = ETag::none();
  
  auto j = Dict::getObject(Dict::parseString(req->body()));
  if (!j) {
    return server->fatal(req, "could not parse body to JSON.");
  }
  BOOST_LOG_TRIVIAL(trace) << Dict::toString(*j);

  // query a stream.
  auto query = Dict::getBool(j, "query");
  if (query && query.value()) {
  
    if (!server->isAdmin(req)) {
      return server->unauthorised(req);
    }
    
    auto name = Dict::getString(j, "name");
    if (!name) {
      return server->fatal(req, "query requires name.");
    }
    
    if (!req->header().has_field("socketid")) {
      return server->fatal(req, "query requires socketid.");
    }
    auto socketid = req->header().value_of("socketid");
    
    auto msg = dictO({
      { "type", "query" },
      { "objtype", "stream" },      
      { "name", name.value() },
      { "corr", string(socketid) }
    });
    server->send(msg);
    auto reply = server->receive();
    
    auto resp = server->checkErrors(req, reply, "query");
    if (resp) {
      return resp.value();
    }
  
//    BOOST_LOG_TRIVIAL(trace) << j;
    
    return server->returnEmptyObj(req, etag);
  }
  
  // add an upstream stream
  auto upstream = Dict::getBool(j, "upstream");
  if (upstream && upstream.value()) {
  
    if (!server->isAdmin(req)) {
      return server->unauthorised(req);
    }

    auto id = Dict::getString(j, "_id");
    if (!id) {
      return server->fatal(req, "upstream requires id.");
    }
    auto msg = dictO({
      { "type", "addobject" },
      { "objtype", "stream" },
      { "id", id.value() },      
      { "upstream", true }
    });
    server->send(msg);
    auto reply = server->receive();
    
    auto resp = server->checkErrors(req, reply, "addstream");
    if (resp) {
      return resp.value();
    }
  
//    BOOST_LOG_TRIVIAL(trace) << j;
    
    return server->returnEmptyObj(req, etag);
  }

  auto msg = dictO({
    { "type", "addobject" },
    { "objtype", "stream" }
  });
  return server->sendBodyReturnEmptyObj(req, msg);

}

};
