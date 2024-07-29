/*
  getstreams.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 26-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>

status_t Server::getstreams(
  const req_t& req, params_t )
{
  auto session = getSession(req);
  if (!session) {
    return unauthorised(req);
  }
  send({ 
    { "type", "streams" },
    { "user", session.value()->userid() }
  });
  json j = receive();
  auto streams = Json::getArray(j, "streams");
  
  if (!streams) {
    // send fatal error
    BOOST_LOG_TRIVIAL(error) << "streams missing streams";
    return init_resp(req->create_response(restinio::status_internal_server_error())).done();
  }

  auto resp = init_resp( req->create_response() );

  boost::json::array newstreams;
  for (auto s: streams.value()) {
    s.as_object()["_id"] = Json::getString(s, "id").value();
    newstreams.push_back(s);
  }
  stringstream ss;
  ss << newstreams;
  resp.set_body(ss.str());

  return resp.done();
}
