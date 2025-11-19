/*
  json.hpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 2-Jul-2024
    
  JSON code for Nodes
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#ifndef H_json
#define H_json

#include "dict.hpp"

#include <string>

using namespace std;
using namespace vops;

class Json {

public:

  static DictV fixIds(const DictV &arr);
  static DictO fixObject(const DictO &j);
  
};

#endif // H_json
