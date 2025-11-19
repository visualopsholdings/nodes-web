/*
  zmqclient.hpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 16-Aug-2024
    
  An ZMQ client for the subscriptions.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#ifndef H_zmqclient
#define H_zmqclient

#include "dict.hpp"

#include <string>
#include <map>
#include <zmq.hpp>

using namespace std;
using namespace vops;

class Server;

typedef function<void (const DictO &)> msgHandler;

class ZMQClient : public enable_shared_from_this<ZMQClient> {

public:
  ZMQClient(Server *server, int sub);
  
  void run();
  
  Server *_server;

private:
  shared_ptr<zmq::context_t> _context;
  shared_ptr<zmq::socket_t> _sub;
  map<string, msgHandler> _submessages;
  
  void receive();

  static void handle_reply(const zmq::message_t &reply, map<string, msgHandler> *handlers);

};

#endif // H_zmqclient
