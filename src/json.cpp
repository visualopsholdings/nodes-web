/*
  json.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 2-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "json.hpp"

#include "date.hpp"

#include <boost/log/trivial.hpp>

DictO Json::fixObject(const DictO &j) {

  DictO newobject;
  for (auto i: j) {
    auto key = get<0>(i);
    auto value = get<1>(i);
    
    auto obj = Dict::getObject(value);
    if (obj) {
      newobject[key] = Json::fixObject(*obj);
      continue;
    }
    auto v = Dict::getVector(value);
    if (v) {
      newobject[key] = Json::fixIds(*v);
      continue;
    }
    if (key == "id") {
      newobject["_id"] = value;
      continue;
    }
    newobject[key] = value;
  }
  return newobject;
  
}

DictV Json::fixIds(const DictV &arr) {

  DictV newarray;
    
  // copy array, recursing into sub objects.
  transform(arr.begin(), arr.end(), back_inserter(newarray), [](auto e) -> DictG {
    auto obj = Dict::getObject(e);
    if (obj) {
      return Json::fixObject(*obj); 
    }
    return e;
  });
  
  return newarray;

}
