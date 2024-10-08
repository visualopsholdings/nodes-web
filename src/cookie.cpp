/*
  cookie.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 8-Jul-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "cookie.hpp"

#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/log/trivial.hpp>

optional<string> Cookie::parseCookie(const string &cookie) {

//  BOOST_LOG_TRIVIAL(trace) << cookie;
  
  vector<string> cookies;
  boost::split(cookies, cookie, boost::is_any_of(";"));
  
  for (auto c: cookies) {
    boost::trim(c);
    vector<string> nvp;
    boost::split(nvp, c, boost::is_any_of("="));
    if (nvp.size() == 2) {
      if (nvp[0] == "ss-id") {
        return nvp[1];
      }
    }
  }
  
  return nullopt;
}
