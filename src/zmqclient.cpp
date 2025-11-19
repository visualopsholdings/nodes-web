/*
  zmqclient.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 16-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "zmqclient.hpp"

#include <boost/log/trivial.hpp>
#include <thread>

namespace nodes {

// messages
void sendWS(ZMQClient *client, const DictO &json);
void sendIdeaWS(ZMQClient *client, const DictO &json);

};

ZMQClient::ZMQClient(Server *server, int subPort) : 
  _server(server) {

  _context.reset(new zmq::context_t(1));
  _sub.reset(new zmq::socket_t(*_context, ZMQ_SUB));
  _sub->connect("tcp://127.0.0.1:" + to_string(subPort));
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)  
  _sub->setsockopt(ZMQ_SUBSCRIBE, 0, 0);
#else
  _sub->set(zmq::sockopt::subscribe, "");
#endif
	BOOST_LOG_TRIVIAL(info) << "Connect to ZMQ as Local SUB on " << subPort;

  // expect these being published
  _submessages["queryResult"] = bind(&nodes::sendWS, this, placeholders::_1);
  _submessages["status"] = bind(&nodes::sendWS, this, placeholders::_1);
  _submessages["idea"] = bind(&nodes::sendIdeaWS, this, placeholders::_1);
  _submessages["nodeSeen"] = bind(&nodes::sendWS, this, placeholders::_1);

}

void ZMQClient::run() {

  thread t(bind(&ZMQClient::receive, this));
  t.detach();
  
}

void ZMQClient::receive() {

  BOOST_LOG_TRIVIAL(trace) << "start receiving";

  zmq::pollitem_t items [] = {
      { *_sub, 0, ZMQ_POLLIN, 0 }
  };
  const std::chrono::milliseconds timeout{500};
  while (1) {
    
//    BOOST_LOG_TRIVIAL(debug) << "polling for messages";
    zmq::message_t message;
    zmq::poll(items, 1, timeout);

//    BOOST_LOG_TRIVIAL(trace) << "sub events " << items[0].revents;

    if (items[0].revents & ZMQ_POLLIN) {
      BOOST_LOG_TRIVIAL(trace) << "got _sub message";
      zmq::message_t reply;
      try {
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
        _sub->recv(&reply);
#else
        auto res = _sub->recv(reply, zmq::recv_flags::none);
#endif
        handle_reply(reply, &_submessages);
      }
      catch (zmq::error_t &e) {
        BOOST_LOG_TRIVIAL(warning) << "got exc with _sub recv " << e.what() << "(" << e.num() << ")";
      }
    }
  }

}

void ZMQClient::handle_reply(const zmq::message_t &reply, map<string, msgHandler> *handlers) {

  BOOST_LOG_TRIVIAL(trace) << "handling reply";

  // convert to JSON
  string r((const char *)reply.data(), reply.size());

  auto doc = Dict::getObject(Dict::parseString(r));
  if (!doc) {
    BOOST_LOG_TRIVIAL(error) << "could not parse body to JSON object.";
    return;
  }
  BOOST_LOG_TRIVIAL(debug) << "<-* " << Dict::toString(*doc);

  // switch the handler based on the message type.
  auto type = Dict::getString(doc, "type");
  if (!type) {
    BOOST_LOG_TRIVIAL(error) << "no type";
    return;
  }

  map<string, msgHandler>::iterator handler = handlers->find(type.value());
  if (handler == handlers->end()) {
    BOOST_LOG_TRIVIAL(error) << "unknown reply type " << type.value();
    return;
  }
  handler->second(*doc);
  
}

