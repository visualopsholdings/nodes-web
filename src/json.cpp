/*
  json.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 2-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
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

json Json::fixObject(const json &j) {

  if (j.is_object()) {
    boost::json::object newobject;
    for (auto i: j.as_object()) {
      if (i.value().is_object()) {
        newobject[i.key()] = Json::fixObject(i.value());
        continue;
      }
      if (i.value().is_array()) {
        newobject[i.key()] = Json::fixIds(i.value().as_array());
        continue;
      }
      if (i.key() == "id") {
        newobject["_id"] = i.value();
        continue;
      }
      newobject[i.key()] = i.value();
    }
    return newobject;
  }

  return {};
  
}

boost::json::array Json::fixIds(boost::json::array &arr) {

    boost::json::array newarray;
    
    // copy array, recursing into sub objects.
    transform(arr.begin(), arr.end(), back_inserter(newarray), [](auto e) {
      if (e.is_object()) {
        return Json::fixObject(e); 
      }
      return e;
    });
    
    return newarray;

}
