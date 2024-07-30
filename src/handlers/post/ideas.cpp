/*
  postideas.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 30-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>

status_t Server::postideas(const req_t& req, params_t params) {

  auto session = getSession(req);
  if (!session) {
    return unauthorised(req);
  }
  json j = boost::json::parse(req->body());
  BOOST_LOG_TRIVIAL(trace) << "postideas " << j;

  auto user = Json::getString(j, "user");
  if (!user) {
    BOOST_LOG_TRIVIAL(error) << "missing user";
    return unauthorised(req);
  }

  if (session.value()->userid() != user.value()) {
    BOOST_LOG_TRIVIAL(trace) << "wrong user";
    return unauthorised(req);
  }
  auto stream = Json::getString(j, "stream");
  if (!stream) {
    BOOST_LOG_TRIVIAL(error) << "missing stream";
    return unauthorised(req);
  }
  auto text= Json::getString(j, "text");
  if (!text) {
    BOOST_LOG_TRIVIAL(error) << "missing text";
    return unauthorised(req);
  }
  
	send({ 
	  { "type", "message" }, 
	  { "user", user.value() },
	  { "stream", stream.value() },
	  { "text", text.value() }
	});
  j = receive();
  
  BOOST_LOG_TRIVIAL(trace) << j;
  // test for an error...
  
  auto resp = req->create_response();
  resp.set_body("{}");
  return resp.done();

}
