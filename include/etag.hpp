/*
  etag.hpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Sep-2024
    
  ETag code for Nodes HTTP Daemon.
  
  ETags look at the "If-None-Match" header and find some base64 encoded JSON
  in there.
  
  If the data means that the browser has the latest, then requests will return
  not-modified.
  
  When the request us returned, data is gathered up for the request as base64 
  encoded JSON and the "Etag" header is set.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#ifndef H_etag
#define H_etag

#include <restinio/http_headers.hpp>
#include <restinio/settings.hpp>
#include <boost/json.hpp>

using namespace std;
using json = boost::json::value;

using req_t = restinio::request_handle_t;
using response_builder_t = restinio::response_builder_t<restinio::restinio_controlled_output_t>;

class ETagHandler {

public:
  virtual bool resultModified(json &j, const string &field) = 0;
    // for those messages that need "test", check the result we get back to see if it's the
    // latest. Otherwise jusyt igore this.
    
  virtual void setHeaders(response_builder_t &resp) = 0;
    // finally set the ETag header. if we need to.
    
};

class Session;

class ETag {

public:

  static shared_ptr<ETagHandler> none();
    // a Null etag.

  static shared_ptr<ETagHandler> simpleTime(const req_t& req, std::shared_ptr<Session> session);
    // an etag that makes sure that a particular user get's the same request in under 1 second.
    
  static shared_ptr<ETagHandler> modifyDate(const req_t& req, std::shared_ptr<Session> session, json *msg);
    // an etag that makes sure that the modify date for an object has changed
    
};

#endif // H_etag
