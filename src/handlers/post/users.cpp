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
  auto etag = ETag::none();
  
  json j = boost::json::parse(req->body());
  BOOST_LOG_TRIVIAL(trace) << j;

  // query a user.
  auto query = Json::getBool(j, "query", true);
  if (query && query.value()) {
  
    // get the name or the fullname.
    auto fullname = Json::getString(j, "fullname", true);
    if (!fullname) {
      return server->fatal(req, "query requires fullname.");
    }
    
    if (!req->header().has_field("socketid")) {
      return server->fatal(req, "query requires socketid.");
    }
    auto socketid = req->header().value_of("socketid");
    
    j = {
      { "type", "query" },
      { "objtype", "user" },      
      { "fullname", fullname.value() },
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
  auto upstream = Json::getBool(j, "upstream", true);
  if (upstream && upstream.value()) {
  
    auto id = Json::getString(j, "_id");
    if (!id) {
      return server->fatal(req, "upstream requires id.");
    }
    j = {
      { "type", "adduser" },
      { "id", id.value() },      
      { "upstream", true }
    };
    server->send(j);
    j = server->receive();
    
    auto resp = server->checkErrors(req, j, "adduser");
    if (resp) {
      return resp.value();
    }
  
//    BOOST_LOG_TRIVIAL(trace) << j;
    
    return server->returnEmptyObj(req, etag);
  }

  return server->fatal(req, "only understand query or upstream.");  

}

};
