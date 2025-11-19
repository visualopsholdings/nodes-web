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

void sendIdeaWS(ZMQClient *client, const DictO &j) {
  
  auto stream = Dict::getString(j, "stream");
  if (!stream) {
    BOOST_LOG_TRIVIAL(error) << "missing stream from idea";
    return;
  }
  
  auto idea = Dict::getString(j, "id");
  if (!idea) {
    BOOST_LOG_TRIVIAL(error) << "missing id from idea";
    return;
  }

  auto corr = Dict::getString(j, "corr");
  
  auto j2 = dictO({ 
    { "type", "update" },
    { "objtype", "stream" },
    { "id", stream.value() },
    { "idea", idea.value() }
  });

  // send to everybody 
  if (corr) {
    client->_server->sendAllWSExcept(j2, corr.value());
  }
  else {
    client->_server->sendAllWS(j2);
  }
  
}

};
