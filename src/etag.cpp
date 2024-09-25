/*
  etag.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Sep-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "etag.hpp"

#include "date.hpp"
#include "json.hpp"
#include "session.hpp"

#include <boost/log/trivial.hpp>
#include <base64.hpp>
#include <boost/json.hpp>

using json = boost::json::value;

class ETagSimpleTime: public ETagHandler {

public:
  ETagSimpleTime(std::shared_ptr<Session> session): _session(session) {}
  
protected:
  virtual void setHeaders(response_builder_t &resp);

private:
  std::shared_ptr<Session> _session;
};

class ETagNone: public ETagHandler {

protected:
  virtual void setHeaders(response_builder_t &resp) {
  }

};

shared_ptr<ETagHandler> ETag::none(const req_t& req) {

  return shared_ptr<ETagHandler>(new ETagNone());
  
}


shared_ptr<ETagHandler> ETag::simpleTime(const req_t& req, std::shared_ptr<Session> session) {

  BOOST_LOG_TRIVIAL(trace) << "simpleTime";

  // test the etag.
  if (req->header().has_field("If-None-Match")) {
    auto etag = req->header().get_field("If-None-Match");
    BOOST_LOG_TRIVIAL(trace) << "sts: " << etag;
    string s = base64::from_base64(etag);
    json j = boost::json::parse(s);
    BOOST_LOG_TRIVIAL(trace) << "sts: " << j;
    auto id = Json::getString(j, "_id");
    auto sts = Json::getNumber(j, "sts");
    long now = Date::now();
    if (id.value() == session->userid() && now <= (sts.value() + 1000)) {
      BOOST_LOG_TRIVIAL(trace) << "sts: same";
      return 0;
    }
  }
  
  return shared_ptr<ETagHandler>(new ETagSimpleTime(session));
  
}

void ETagSimpleTime::setHeaders(response_builder_t &resp) {

  json j = {
    { "_id", _session->userid() },
    { "sts", Date::now() }
  };
  BOOST_LOG_TRIVIAL(trace) << "setting headers: " << j;

  stringstream ss;
  ss << j;
  string etag = base64::to_base64(ss.str());
  BOOST_LOG_TRIVIAL(trace) << "setting Etag " << etag;
  resp.append_header("Etag", etag);
  
}
 