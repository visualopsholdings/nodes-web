/*
  path.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 9-Oct-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "path.hpp"


optional<string> Path::getAppPath(const string &path) {

  if (path.find("/apps/") != string::npos) {
    return"frontend/" + path;
  }
  
  return nullopt;
  
}
