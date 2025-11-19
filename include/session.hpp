/*
  session.hpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 9-Jul-2024
    
  Session management for Nodes HTTP Daemon
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#ifndef H_session
#define H_session

#include "dict.hpp"

#include <string>
#include <map>
#include <memory>

using namespace std;
using namespace vops;

class Session {

public:
  Session(const DictO &json);
  
  bool isAdmin() { return _admin; };
  string userid() { return _userid; };
  string name() { return _name; };
  string fullname() { return _fullname; };
  bool admin() { return _admin; };
  string modifyDate() { return _modifyDate; };

private:
  string _userid;
  string _name;
  string _fullname;
  string _modifyDate;
  bool _admin;
  
};

class Sessions {

public:

  static shared_ptr<Sessions> instance() {
    if(!_instance) {
      _instance.reset(new Sessions());
    }
    return _instance;
  };
    
  string create(const DictO &json);

  bool has(const string &sessid) {
    return _sessions.find(sessid) != _sessions.end();
  }
  shared_ptr<Session> get(const string &sessid) {
    return _sessions[sessid];
  }
  void destroy(const string &sessid) {
    _sessions.erase(sessid);
  }
  
private:

  // there can be only 1.
  Sessions() {};
  static shared_ptr<Sessions> _instance;

  map<string, shared_ptr<Session> > _sessions;
  
};

#endif // H_session
