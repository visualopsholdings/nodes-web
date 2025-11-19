/*
  testjson.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 21-May-2024
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include <iostream>

#include "json.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE( fixIds )
{
  cout << "=== fixIds ===" << endl;
  
  auto j = DictV{
    dictO({ 
      { "id", "aaaaa" },
      { "other", "yyyy" }
    }),
    dictO({ 
      { "id", "bbbb" },
      { "other", "zzzz" }
    })
  };
  auto fixed = Json::fixIds(j);
  cout << Dict::toString(fixed) << endl;
  
}
