/*
  testcookie.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 8-Jul-2024
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/zmqchat
*/

#include "cookie.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE( parseCookie )
{
  cout << "=== parseCookie ===" << endl;
  
  auto id = Cookie::parseCookie("io=q929Z4KofUWo4Q4JAAAE; ss-id=667d0baedfb1ed18430d8ed3");
  BOOST_CHECK(id);
  BOOST_CHECK_EQUAL(id.value(), "667d0baedfb1ed18430d8ed3");

}
