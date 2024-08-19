/*
  websocket.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 19-Aug-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>
#include <restinio/websocket/websocket.hpp>

using router_t = restinio::router::express_router_t<>;
using traits_t =
    restinio::traits_t<
       restinio::asio_timer_manager_t,
//        restinio::single_threaded_ostream_logger_t,
       restinio::null_logger_t,
       router_t >;

namespace nodes {

status_t websocket(Server *server, const req_t& req, params_t params)
{
  BOOST_LOG_TRIVIAL(trace) << "in socket";  

  if (restinio::http_connection_header_t::upgrade == req->header().connection()) {
  
    BOOST_LOG_TRIVIAL(trace) << "upgrading";
    
    shared_ptr<rws::ws_t> wsh =
      rws::upgrade< traits_t >(
        *req,
        rws::activation_t::immediate,
        [](shared_ptr<rws::ws_t> wsh, shared_ptr<rws::message_t> m){
        
          BOOST_LOG_TRIVIAL(trace) << "got " << m->payload();;
          
        } );
         
    // send the ID.
    json j = {
      { "type", "id" },
      { "id", wsh->connection_id() }
    };
    stringstream ss;
    ss << j;
    wsh->send_message(rws::message_t(rws::final_frame, rws::opcode_t::text_frame, ss.str()));

    // save ID:    
    server->_registry.emplace(wsh->connection_id(), wsh);

    return restinio::request_accepted();
  }

  return restinio::request_rejected();
}

};


