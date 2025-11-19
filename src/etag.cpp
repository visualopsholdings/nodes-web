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

class ETagNone: public ETagHandler {

public:
  ETagNone(): ETagHandler(nullopt) {}

protected:
  virtual bool modified() { return false; };
  virtual bool resultModified(const DictO &j, const string &field) { return false; }
  virtual void setHeaders(response_builder_t &resp) {}

};

class ETagSimpleTime: public ETagHandler {

public:
  ETagSimpleTime(std::shared_ptr<Session> session, optional<string> origEtag): ETagHandler(origEtag), _session(session) {}
  
protected:
  virtual bool modified();
  virtual void setHeaders(response_builder_t &resp);
  virtual bool resultModified(const DictO &j, const string &field) { return false; }

private:
  std::shared_ptr<Session> _session;
};

class ETagModifyDate: public ETagHandler {

public:
  ETagModifyDate(optional<string> origEtag): ETagHandler(origEtag) {}

protected:
  
  virtual bool modified() { return false; };
  virtual bool resultModified(const DictO &j, const string &field);
  virtual void setHeaders(response_builder_t &resp);

private:
  string _time;
  
};

class ETagCollectionChanged: public ETagHandler {

public:
  ETagCollectionChanged(optional<string> origEtag): ETagHandler(origEtag) {}

protected:
  
  virtual bool modified() { return false; };
  virtual bool resultModified(const DictO &j, const string &field);
  virtual void setHeaders(response_builder_t &resp);

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
    return shared_ptr<ETagHandler>(new ETagSimpleTime(session, etag));
  }
  
  return shared_ptr<ETagHandler>(new ETagSimpleTime(session, nullopt));
  
}

bool ETagSimpleTime::modified() {

  if (_origEtag) {
    // test the original etag.
    string s = base64::from_base64(_origEtag.value());
    auto j = Dict::getObject(Dict::parseString(s));
    if (!j) {
      BOOST_LOG_TRIVIAL(error) << "etag contents not json object " << s;
      return false;
    }
//    BOOST_LOG_TRIVIAL(trace) << "JSON " << Dict::toString(*j);
    auto user = Dict::getString(*j, "user");
    if (!user) {
      BOOST_LOG_TRIVIAL(error) << "no user found";
      return false;
    }
    auto time = Dict::getNum(*j, "time");
    if (!time) {
      BOOST_LOG_TRIVIAL(error) << "no time found";
      return false;
    }
    long now = Date::now();
    BOOST_LOG_TRIVIAL(trace) << "here3";
    return user.value() != _session->userid() || now > (time.value() + 1000);
  }

  // no etag, so assume it's modified
  return true;
  
}

void ETagSimpleTime::setHeaders(response_builder_t &resp) {

  auto j = dictO({
    { "user", _session->userid() },
    { "time", Date::now() }
  });
  auto s = Dict::toString(j, false);
  BOOST_LOG_TRIVIAL(trace) << "setting headers: " << s;

  string etag = base64::to_base64(s);
  BOOST_LOG_TRIVIAL(trace) << "setting Etag " << etag;
  resp.append_header("Etag", etag);
  
}
 
shared_ptr<ETagHandler> ETag::modifyDate(const req_t& req, DictO *msg) {

  BOOST_LOG_TRIVIAL(trace) << "modifyDate";

  // get the JSON out of the etag
  if (req->header().has_field("If-None-Match")) {
    auto etag = req->header().get_field("If-None-Match");
    BOOST_LOG_TRIVIAL(trace) << "modifyDate: " << etag;
    auto s = base64::from_base64(etag);
    auto j = Dict::parseString(s);
    if (j) {
      (*msg)["test"] = *j;
    }
    else {
      BOOST_LOG_TRIVIAL(error) << "etag contents not json " << s << " setting test empty";
      (*msg)["test"] = DictO();
    }
    return shared_ptr<ETagHandler>(new ETagModifyDate(etag));
  }
  
  return shared_ptr<ETagHandler>(new ETagModifyDate(nullopt));
  
}

bool ETagModifyDate::resultModified(const DictO &j, const string &field) {

  auto test = Dict::getObject(j, "test");
  if (test) {
    auto latest = Dict::getBool(test.value(), "latest");
    if (latest && latest.value()) {
      return true;
    }
  }

  auto obj = Dict::getObject(j, field);
  if (!obj) {
    BOOST_LOG_TRIVIAL(trace) << "missing " << field;
    return false;
  }
  
  // remember the time always.
  auto mod = Dict::getString(obj.value(), "modifyDate");
  if (mod) {
    _time = mod.value();
    //  BOOST_LOG_TRIVIAL(trace) << _time;
  }
  
  return false;
}

void ETagModifyDate::setHeaders(response_builder_t &resp) {

  auto j = dictO({
    { "modifyDate", _time }
  });
  string s = Dict::toString(j);
  BOOST_LOG_TRIVIAL(trace) << "setting headers: " << s;

  string etag = base64::to_base64(s);
  BOOST_LOG_TRIVIAL(trace) << "setting Etag " << etag;
  resp.append_header("Etag", etag);

}

shared_ptr<ETagHandler> ETag::collectionChanged(const req_t& req, DictO *msg) {

  BOOST_LOG_TRIVIAL(trace) << "collectionChanged";

  // get the JSON out of the etag
  if (req->header().has_field("If-None-Match")) {
    auto etag = req->header().get_field("If-None-Match");
    BOOST_LOG_TRIVIAL(trace) << "collectionChanged: " << etag;
    auto s = base64::from_base64(etag);
    auto j = Dict::parseString(s);
    if (j) {
      (*msg)["test"] = *j;
    }
    else {
      BOOST_LOG_TRIVIAL(error) << "etag contents not json " << s << " setting test empty";
      (*msg)["test"] = DictO();
    }
    return shared_ptr<ETagHandler>(new ETagCollectionChanged(etag));
  }
  
  return shared_ptr<ETagHandler>(new ETagCollectionChanged(nullopt));
  
}

bool ETagCollectionChanged::resultModified(const DictO &j, const string &field) {

  auto test = Dict::getObject(j, "test");
  if (test) {
    auto latest = Dict::getBool(test.value(), "latest");
    if (latest && latest.value()) {
      return true;
    }
  }

  // remember the time
  auto mod = Dict::getNum(test.value(), "time");
  if (mod) {
    _time = mod.value();
  //  BOOST_LOG_TRIVIAL(trace) << _time;
  }

  return false;
}

void ETagCollectionChanged::setHeaders(response_builder_t &resp) {

  auto j = dictO({
    { "time", _time }
  });
  string s = Dict::toString(j);
  BOOST_LOG_TRIVIAL(trace) << "setting headers: " << s;

  string etag = base64::to_base64(s);
  BOOST_LOG_TRIVIAL(trace) << "setting Etag " << etag;
  resp.append_header("Etag", etag);

}

    
