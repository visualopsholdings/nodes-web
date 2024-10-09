/*
  testpath.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 9-Oct-2024
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include <iostream>

#include "path.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE( parseAppFile )
{
  cout << "=== parseAppFile ===" << endl;
  
  string path = "/apps/rchat/static/css/main.bba7c994.css";
  
  auto appPath = Path::getAppPath(path);
  
  BOOST_CHECK(appPath);
  BOOST_CHECK_EQUAL(appPath.value(), "frontend/" + path);
  
}
