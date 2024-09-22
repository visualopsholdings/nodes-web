/*
  sendIdeaWS.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 21-Sep-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "zmqclient.hpp"

#include "server.hpp"
#include "json.hpp"

#include <boost/log/trivial.hpp>

namespace nodes {

void sendIdeaWS(ZMQClient *client, json &j) {
  
  auto stream = Json::getString(j, "stream");
  if (!stream) {
    BOOST_LOG_TRIVIAL(error) << "missing stream from idea";
    return;
  }
  
  auto idea = Json::getString(j, "id");
  if (!idea) {
    BOOST_LOG_TRIVIAL(error) << "missing id from idea";
    return;
  }

  auto socketid = Json::getString(j, "socketid");
  
  j = { 
    { "type", "update" },
    { "objtype", "stream" },
    { "id", stream.value() },
    { "idea", idea.value() }
  };

  // send to everybody 
  if (socketid) {
    client->_server->sendAllWSExcept(j, socketid.value());
  }
  else {
    client->_server->sendAllWS(j);
  }
  
}

};
