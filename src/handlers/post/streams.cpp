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
  
  json j = boost::json::parse(req->body());
  BOOST_LOG_TRIVIAL(trace) << j;

  // query a stream.
  auto query = Json::getBool(j, "query", true);
  if (query && query.value()) {
  
    if (!server->isAdmin(req)) {
      return server->unauthorised(req);
    }
    
    auto name = Json::getString(j, "name");
    if (!name) {
      return server->fatal(req, "query requires name.");
    }
    
    if (!req->header().has_field("socketid")) {
      return server->fatal(req, "query requires socketid.");
    }
    auto socketid = req->header().value_of("socketid");
    
    j = {
      { "type", "query" },
      { "objtype", "stream" },      
      { "name", name.value() },
      { "corr", socketid }
    };
    server->send(j);
    j = server->receive();
    
    auto resp = server->checkErrors(req, j, "query");
    if (resp) {
      return resp.value();
    }
  
//    BOOST_LOG_TRIVIAL(trace) << j;
    
    return server->returnEmptyObj(req, etag);
  }
  
  // add an upstream stream
  auto upstream = Json::getBool(j, "upstream", true);
  if (upstream && upstream.value()) {
  
    if (!server->isAdmin(req)) {
      return server->unauthorised(req);
    }

    auto id = Json::getString(j, "_id");
    if (!id) {
      return server->fatal(req, "upstream requires id.");
    }
    j = {
      { "type", "addstream" },
      { "id", id.value() },      
      { "upstream", true }
    };
    server->send(j);
    j = server->receive();
    
    auto resp = server->checkErrors(req, j, "addstream");
    if (resp) {
      return resp.value();
    }
  
//    BOOST_LOG_TRIVIAL(trace) << j;
    
    return server->returnEmptyObj(req, etag);
  }

  return server->sendBodyReturnEmptyObj(req, "addstream");

}

};
