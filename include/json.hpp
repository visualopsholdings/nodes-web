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

#include <string>
#include <boost/json.hpp>

using namespace std;
using json = boost::json::value;

class Json {

public:

  static string toISODate(json &date);
  static bool has(const json &j, const string &name);
  static optional<string> getString(const json &j, const string &name, bool silent=false);
  static optional<boost::json::array> getArray(json &reply, const string &name, bool silent=false);
  static optional<json> getObject(json &j, const string &name, bool silent=false);
  static optional<bool> getBool(const json &j, const string &name, bool silent=false);
  static optional<long> getNumber(const json &j, const string &name, bool silent=false);
  static boost::json::array fixIds(boost::json::array &arr);
  static json fixObject(const json &j);
  
private:

  static json getMember(const json &j, const string &name, bool silent);

};

#endif // H_json
