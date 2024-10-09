/*
  path.hpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 9-Oct-2024
    
  Dealing with file paths.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#ifndef H_path
#define H_path

#include <optional>
#include <string>

using namespace std;

class Path {

public:
  
  static optional<string> getAppPath(const string &path);

};

#endif // H_path
