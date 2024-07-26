/*
  session.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 9-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/czhttp
*/

#include "session.hpp"

#include "json.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <boost/log/trivial.hpp>

using namespace std;

shared_ptr<Sessions> Sessions::_instance;

Session::Session(json json) {
  
  auto id = Json::getString(json, "id");
  if (id) {
    _userid = id.value();
  }
  else {
	  BOOST_LOG_TRIVIAL(error) << "missing id ";
  }

  auto name = Json::getString(json, "name");
  if (name) {
    _name = name.value();
  }

  auto fullname = Json::getString(json, "fullname");
  if (fullname) {
    _fullname = fullname.value();
  }

  auto admin = Json::getBool(json, "admin");
  if (admin) {
    _admin = admin.value();
  }
  
}

string Sessions::create(json json) {

  auto uuid = boost::uuids::to_string(boost::uuids::random_generator()());
  
  _sessions[uuid] = shared_ptr<Session>(new Session(json));
  
  return uuid;
  
}
