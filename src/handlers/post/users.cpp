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

status_t postusers(Server *server, const req_t& req, params_t params) {

  if (!server->isAdmin(req)) {
    return server->unauthorised(req);
  }
  auto etag = ETag::none(req);
  if (!etag) {
    return server->not_modified(req);
  }
  
  json j = boost::json::parse(req->body());
  BOOST_LOG_TRIVIAL(trace) << j;

  // query a user.
  auto query = Json::getBool(j, "query");
  if (query && query.value()) {
    auto email = Json::getString(j, "email");
    if (!email) {
      return server->fatal(req, "query requires email.");
    }
    
    if (!req->header().has_field("socketid")) {
      return server->fatal(req, "query requires socketid.");
    }
    auto socketid = req->header().value_of("socketid");
    
    j = {
      { "type", "query" },
      { "objtype", "user" },      
      { "email", email.value() },
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
  
  // add an upstream user
  auto upstream = Json::getBool(j, "upstream");
  if (upstream && upstream.value()) {
  
    auto id = Json::getString(j, "_id");
    if (!id) {
      return server->fatal(req, "upstream requires id.");
    }
    j = {
      { "type", "newuser" },
      { "id", id.value() },      
      { "upstream", true }
    };
    server->send(j);
    j = server->receive();
    
    auto resp = server->checkErrors(req, j, "newuser");
    if (resp) {
      return resp.value();
    }
  
//    BOOST_LOG_TRIVIAL(trace) << j;
    
    return server->returnEmptyObj(req, etag);
  }

  return server->fatal(req, "only understand query or upstream.");  

}

};
