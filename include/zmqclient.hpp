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

#include <string>
#include <map>
#include <boost/json.hpp>
#include <zmq.hpp>

using namespace std;
using json = boost::json::value;

class Server;

typedef function<void (json &)> msgHandler;

class ZMQClient : public enable_shared_from_this<ZMQClient> {

public:
  ZMQClient(Server *server, int sub);
  
  void run();
  
private:
  Server *_server;
  shared_ptr<zmq::context_t> _context;
  shared_ptr<zmq::socket_t> _sub;
  map<string, msgHandler> _submessages;
  
  void receive();

  static void handle_reply(const zmq::message_t &reply, map<string, msgHandler> *handlers);

};

#endif // H_zmqclient
