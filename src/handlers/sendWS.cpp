/*
  sendWS.cpp
  
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

void sendWS(ZMQClient *client, const DictO &json) {
  
  BOOST_LOG_TRIVIAL(trace) << "sendWS";
  
  auto corr = Dict::getString(json, "corr");
  if (!corr) {
    // send to all sockets.
    client->_server->sendAllWS(Json::fixObject(json));
    return;
  }

  // send to the one socket.
  uint64_t id = stol(corr.value());
  client->_server->sendWS(id, Json::fixObject(json));
  
}

};
