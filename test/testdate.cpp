/*
  testdate.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Sep-2024
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes
*/

#include <iostream>

#include "date.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace std;
using json = boost::json::value;

BOOST_AUTO_TEST_CASE( toISODate )
{
  cout << "=== toISODate ===" << endl;
  
  BOOST_CHECK_EQUAL(Date::toISODate(1721890479599), "2024-07-25T06:54:39.599+00:00");
  
}
