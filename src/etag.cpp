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

class ETagSimpleTime: public ETagHandler {

public:
  ETagSimpleTime(std::shared_ptr<Session> session): ETagHandler(nullopt), _session(session) {}
  
protected:
  virtual void setHeaders(response_builder_t &resp);
  virtual bool resultModified(json &j, const string &field) { return false; }

private:
  std::shared_ptr<Session> _session;
};

class ETagNone: public ETagHandler {

public:
  ETagNone(): ETagHandler(nullopt) {}

protected:
  virtual void setHeaders(response_builder_t &resp) {}
  virtual bool resultModified(json &j, const string &field) { return false; }

};

class ETagModifyDate: public ETagHandler {

public:
  ETagModifyDate(optional<string> origEtag): ETagHandler(origEtag) {}

protected:
  
  virtual void setHeaders(response_builder_t &resp);
  virtual bool resultModified(json &j, const string &field);

private:
  string _time;
  
};

class ETagCollectionChanged: public ETagHandler {

public:
  ETagCollectionChanged(optional<string> origEtag): ETagHandler(origEtag) {}

protected:
  
  virtual void setHeaders(response_builder_t &resp);
  virtual bool resultModified(json &j, const string &field);

private:
  long _time;
  
};

shared_ptr<ETagHandler> ETag::none() {

  return shared_ptr<ETagHandler>(new ETagNone());
  
}

shared_ptr<ETagHandler> ETag::simpleTime(const req_t& req, std::shared_ptr<Session> session) {

  BOOST_LOG_TRIVIAL(trace) << "simpleTime";

  // test the etag.
  if (req->header().has_field("If-None-Match")) {
    auto etag = req->header().get_field("If-None-Match");
    BOOST_LOG_TRIVIAL(trace) << etag;
    string s = base64::from_base64(etag);
    json j = boost::json::parse(s);
    BOOST_LOG_TRIVIAL(trace) << j;
    auto user = Json::getString(j, "user");
    auto time = Json::getNumber(j, "time");
    long now = Date::now();
    if (user.value() == session->userid() && now <= (time.value() + 1000)) {
      BOOST_LOG_TRIVIAL(trace) << "same";
      return 0;
    }
  }
  
  return shared_ptr<ETagHandler>(new ETagSimpleTime(session));
  
}

void ETagSimpleTime::setHeaders(response_builder_t &resp) {

  json j = {
    { "user", _session->userid() },
    { "time", Date::now() }
  };
  BOOST_LOG_TRIVIAL(trace) << "setting headers: " << j;

  stringstream ss;
  ss << j;
  string etag = base64::to_base64(ss.str());
  BOOST_LOG_TRIVIAL(trace) << "setting Etag " << etag;
  resp.append_header("Etag", etag);
  
}
 
shared_ptr<ETagHandler> ETag::modifyDate(const req_t& req, json *msg) {

  BOOST_LOG_TRIVIAL(trace) << "modifyDate";

  // get the JSON out of the etag
  if (req->header().has_field("If-None-Match")) {
    auto etag = req->header().get_field("If-None-Match");
    BOOST_LOG_TRIVIAL(trace) << "modifyDate: " << etag;
    (*msg).as_object()["test"] = boost::json::parse(base64::from_base64(etag));
    return shared_ptr<ETagHandler>(new ETagModifyDate(etag));
  }
  
  return shared_ptr<ETagHandler>(new ETagModifyDate(nullopt));
  
}

bool ETagModifyDate::resultModified(json &j, const string &field) {

  auto test = Json::getObject(j, "test", true);
  if (test) {
    auto latest = Json::getBool(test.value(), "latest", true);
    if (latest && latest.value()) {
      return true;
    }
  }

  auto obj = Json::getObject(j, field);
  if (!obj) {
    BOOST_LOG_TRIVIAL(trace) << "missing " << field;
    return false;
  }
  
  // remember the time always.
  auto mod = Json::getString(obj.value(), "modifyDate", true);
  if (mod) {
    _time = mod.value();
    //  BOOST_LOG_TRIVIAL(trace) << _time;
  }
  
  return false;
}

void ETagModifyDate::setHeaders(response_builder_t &resp) {

  json j = {
    { "time", _time }
  };
  BOOST_LOG_TRIVIAL(trace) << "setting headers: " << j;

  stringstream ss;
  ss << j;
  string etag = base64::to_base64(ss.str());
  BOOST_LOG_TRIVIAL(trace) << "setting Etag " << etag;
  resp.append_header("Etag", etag);

}

shared_ptr<ETagHandler> ETag::collectionChanged(const req_t& req, json *msg) {

  BOOST_LOG_TRIVIAL(trace) << "collectionChanged";

  // get the JSON out of the etag
  if (req->header().has_field("If-None-Match")) {
    auto etag = req->header().get_field("If-None-Match");
    BOOST_LOG_TRIVIAL(trace) << "collectionChanged: " << etag;
    (*msg).as_object()["test"] = boost::json::parse(base64::from_base64(etag));
    return shared_ptr<ETagHandler>(new ETagCollectionChanged(etag));
  }
  
  return shared_ptr<ETagHandler>(new ETagCollectionChanged(nullopt));
  
}

bool ETagCollectionChanged::resultModified(json &j, const string &field) {

  auto test = Json::getObject(j, "test", true);
  if (test) {
    auto latest = Json::getBool(test.value(), "latest", true);
    if (latest && latest.value()) {
      return true;
    }
  }

  // remember the time
  auto mod = Json::getNumber(test.value(), "time", true);
  if (mod) {
    _time = mod.value();
  //  BOOST_LOG_TRIVIAL(trace) << _time;
  }

  return false;
}

void ETagCollectionChanged::setHeaders(response_builder_t &resp) {

  json j = {
    { "time", _time }
  };
  BOOST_LOG_TRIVIAL(trace) << "setting headers: " << j;

  stringstream ss;
  ss << j;
  string etag = base64::to_base64(ss.str());
  BOOST_LOG_TRIVIAL(trace) << "setting Etag " << etag;
  resp.append_header("Etag", etag);

}

    
