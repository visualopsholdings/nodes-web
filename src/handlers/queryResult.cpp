/*
  queryResult.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 16-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "zmqclient.hpp"

#include "server.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>

namespace nodes {

void queryResult(ZMQClient *client, json &json) {
  
  BOOST_LOG_TRIVIAL(trace) << "queryResult " << json;

  auto socketid = Json::getString(json, "socketid");
  if (!socketid) {
    BOOST_LOG_TRIVIAL(error) << "missing socketid";
    return;
  }

  uint64_t id = stol(socketid.value());
  client->_server->sendWS(id, json);
  
}

};
