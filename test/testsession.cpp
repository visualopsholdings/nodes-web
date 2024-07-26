/*
  testsession.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 9-Jul-2024
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/zmqchat
*/

#include <iostream>

#include "session.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE( missing )
{
  cout << "=== missing ===" << endl;
  
  BOOST_CHECK(!Sessions::instance()->has("xxxxx"));
  
}

BOOST_AUTO_TEST_CASE( createAndAccess )
{
  cout << "=== create ===" << endl;
  
  json j = {
    { "id", "aaaa" },
    { "admin", true }
  };
  auto session = Sessions::instance()->create(j);
  
  BOOST_CHECK_EQUAL(session.size(), 36);
  BOOST_CHECK(Sessions::instance()->has(session));
  BOOST_CHECK(Sessions::instance()->get(session)->isAdmin())
  ;
  
}
