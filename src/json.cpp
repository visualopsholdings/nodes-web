/*
  json.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 2-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/czhttp
*/

#include "json.hpp"

#include <sstream>
#include <ctime>
#include <iomanip>
#include <boost/log/trivial.hpp>

string Json::toISODate(json &date) {

  if (!date.is_object()) {
    return "not_object";
  }
  
  if (!date.as_object().if_contains("$date")) {
    return "bad_object";
  }
  
  time_t ts = date.at("$date").as_int64();

  time_t tnum = ts / 1000;
  int secs = ts - (tnum * 1000);

  tm tm = *gmtime(&tnum);
  stringstream ss;
  ss << put_time(&tm, "%FT%T.");
  ss << secs;
  ss << "+00:00";

  return ss.str();
  
}

optional<string> Json::getString(json &j, const string &name) {

  if (!j.is_object()) {
    BOOST_LOG_TRIVIAL(error) << "json is not object";
    return {};
  }
  if (!j.as_object().if_contains(name)) {
    return {};
  }
  auto obj = j.at(name);
  if (!obj.is_string()) {
    BOOST_LOG_TRIVIAL(error) << "obj is not string";
    return {};
  }
  return boost::json::value_to<string>(obj);

}

optional<boost::json::array> Json::getArray(json &j, const string &name) {

  if (!j.is_object()) {
    BOOST_LOG_TRIVIAL(error) << "json is not object";
    return {};
  }
  if (!j.as_object().if_contains(name)) {
    return {};
  }
  auto obj = j.at(name);
  if (!obj.is_array()) {
    BOOST_LOG_TRIVIAL(error) << "obj is not array";
    return {};
  }
  return obj.as_array();
  
}

optional<json> Json::getObject(json &j, const string &name) {

  if (!j.is_object()) {
    BOOST_LOG_TRIVIAL(error) << "json is not object";
    return {};
  }
  if (!j.as_object().if_contains(name)) {
    return {};
  }
  auto obj = j.at(name);
  if (!obj.is_object()) {
    BOOST_LOG_TRIVIAL(error) << "obj is not object";
    return {};
  }
  return obj;
  
}

optional<bool> Json::getBool(json &j, const string &name) {

  if (!j.is_object()) {
    BOOST_LOG_TRIVIAL(error) << "json is not object";
    return {};
  }
  if (!j.as_object().if_contains(name)) {
    return {};
  }
  auto obj = j.at(name);
  if (!obj.is_bool()) {
    BOOST_LOG_TRIVIAL(error) << "obj is not bool";
    return {};
  }
  return boost::json::value_to<bool>(obj);

}


