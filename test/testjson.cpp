/*
  testjson.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 21-May-2024
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/zmqchat
*/

#include <iostream>

#include "json.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace std;
using json = boost::json::value;

BOOST_AUTO_TEST_CASE( goodString )
{
  cout << "=== goodString ===" << endl;
  
  json j = {
    { "str", "a string" }
  };
  auto str = Json::getString(j, "str");
  BOOST_CHECK(str);
  BOOST_CHECK_EQUAL(str.value(), "a string");
  
}

BOOST_AUTO_TEST_CASE( badString )
{
  cout << "=== badString ===" << endl;
  
  json j = {
    { "num", 1.1 }
  };
  auto num = Json::getString(j, "num");
  BOOST_CHECK(!num);

}

BOOST_AUTO_TEST_CASE( missing )
{
  cout << "=== missing ===" << endl;
  
  json j = {
    { "xxx", 1.1 }
  };
  auto num = Json::getString(j, "num");
  BOOST_CHECK(!num);

}

BOOST_AUTO_TEST_CASE( goodBool )
{
  cout << "=== goodBool ===" << endl;
  
  json j = {
    { "xxx", true }
  };
  auto xxx = Json::getBool(j, "xxx");
  BOOST_CHECK(xxx);

}

BOOST_AUTO_TEST_CASE( badBool )
{
  cout << "=== badBool ===" << endl;
  
  json j = {
    { "xxx", "yyyy" }
  };
  auto xxx = Json::getBool(j, "xxx");
  BOOST_CHECK(!xxx);

}

BOOST_AUTO_TEST_CASE( goodArray )
{
  cout << "=== goodArray ===" << endl;
  
  json j = {
    { "xxx", { {
      { "aaa", 42 }
    } } }
  };
  auto xxx = Json::getArray(j, "xxx");
  BOOST_CHECK(xxx);
  BOOST_CHECK_EQUAL(xxx.value().size(), 1);
  BOOST_CHECK(xxx.value()[0].is_object());
  BOOST_CHECK(xxx.value()[0].at("aaa").is_int64());
  BOOST_CHECK_EQUAL(xxx.value()[0].at("aaa").as_int64(), 42);

}

BOOST_AUTO_TEST_CASE( badArray )
{
  cout << "=== badArray ===" << endl;
  
  json j = {
    { "xxx", "yyyy" }
  };
  auto xxx = Json::getArray(j, "xxx");
  BOOST_CHECK(!xxx);

}

BOOST_AUTO_TEST_CASE( goodObject )
{
  cout << "=== goodObject ===" << endl;
  
  json j = {
    { "xxx", {
      { "aaa", 42 }
    } }
  };
  auto xxx = Json::getObject(j, "xxx");
  BOOST_CHECK(xxx);
  BOOST_CHECK(xxx.value().is_object());
  BOOST_CHECK(xxx.value().at("aaa").is_int64());
  BOOST_CHECK_EQUAL(xxx.value().at("aaa").as_int64(), 42);

}

BOOST_AUTO_TEST_CASE( badObject )
{
  cout << "=== badObject ===" << endl;
  
  json j = {
    { "xxx", "yyyy" }
  };
  auto xxx = Json::getObject(j, "xxx");
  BOOST_CHECK(!xxx);

}



