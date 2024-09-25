/*
  date.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Sep-2024
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "date.hpp"

#include <sstream>
#include <ctime>
#include <iomanip>
#include <boost/log/trivial.hpp>
#include <cstdlib>

#include "date.hpp"

#include <sstream>
#include <ctime>
#include <iomanip>
#include <boost/log/trivial.hpp>
#include <cstdlib>

#define TIME_FORMAT "%Y-%m-%dT%H:%M:%S"

long Date::now() {

  return chrono::system_clock::to_time_t(chrono::system_clock::now()) * 1000;

}

string Date::toISODate(long t) {

  time_t tnum = t / 1000;
  tm tm = *gmtime(&tnum);
  
//   BOOST_LOG_TRIVIAL(trace) << "tm_sec " << tm.tm_sec;
//   BOOST_LOG_TRIVIAL(trace) << "tm_min " << tm.tm_min;
//   BOOST_LOG_TRIVIAL(trace) << "tm_hour " << tm.tm_hour;
//   BOOST_LOG_TRIVIAL(trace) << "tm_mday " << tm.tm_mday;
//   BOOST_LOG_TRIVIAL(trace) << "tm_mon " << tm.tm_mon;
//   BOOST_LOG_TRIVIAL(trace) << "tm_year " << tm.tm_year;
//   BOOST_LOG_TRIVIAL(trace) << "tm_wday " << tm.tm_wday;
//   BOOST_LOG_TRIVIAL(trace) << "tm_yday " << tm.tm_yday;
//   BOOST_LOG_TRIVIAL(trace) << "tm_isdst " << tm.tm_isdst;

  int ms = t - (tnum * 1000);

  stringstream ss;
  // 2024-07-01T06:54:39
  ss << put_time(&tm, TIME_FORMAT);
  ss << ".";
  ss << ms;
  ss << "+00:00";

  return ss.str();
  
}
