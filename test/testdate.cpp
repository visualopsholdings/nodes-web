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

BOOST_AUTO_TEST_CASE( toISODate )
{
  cout << "=== toISODate ===" << endl;
  
  BOOST_CHECK_EQUAL(Date::toISODate(1721890479599), "2024-07-25T06:54:39.599+00:00");
  
}

BOOST_AUTO_TEST_CASE( fromISODate )
{
  cout << "=== fromISODate ===" << endl;
  
  BOOST_CHECK_EQUAL(Date::fromISODate("2024-07-25T06:54:39.599+00:00"), 1721890479599);
  
}

BOOST_AUTO_TEST_CASE( fromISODateShortMS )
{
  cout << "=== fromISODateShortMS ===" << endl;
  
  BOOST_CHECK_EQUAL(Date::fromISODate("2024-09-18T11:11:30.2+00:00"), 1726657890002);
  
}

BOOST_AUTO_TEST_CASE( inPast )
{
  cout << "=== inPast ===" << endl;
  
  BOOST_CHECK(Date::fromISODate("2024-07-01T06:54:39.599+00:00") < Date::now());
  BOOST_CHECK(Date::fromISODate("2027-07-01T06:54:39.599+00:00") > Date::now());
  
}

