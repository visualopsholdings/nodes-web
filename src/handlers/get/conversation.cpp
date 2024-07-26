/*
  getconversation.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 26-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/czhttp
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

status_t Server::getconversation(
  const req_t& req, params_t params)
{
  auto session = getSession(req);
  if (!session) {
    return unauthorised(req);
  }
  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    auto resp = init_resp( req->create_response() );
    resp.set_body("[]");
    return resp.done();
  }
  send({ 
    { "type", "ideas" },
    { "stream", id }
  });
  json j = receive();
  auto ideas = Json::getArray(j, "ideas");

  if (!ideas) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "conversation missing ideas";
    return init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

  boost::json::array newideas;
  for (auto s: ideas.value()) {
    s.as_object()["_id"] = Json::getString(s, "id").value();
    newideas.push_back(s);
  }

  auto resp = init_resp( req->create_response() );
  stringstream ss;
  ss << newideas;
  resp.set_body(ss.str());

  return resp.done();
}
