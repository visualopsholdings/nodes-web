/*
  date.hpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Sep-2024
    
  Date code for Nodes,
  
  ISO dates look likt this:
    2024-07-25T06:54:39.599+00:00
  Our internal format is this as a time_t, but in milliseconds not seconds.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#ifndef H_date
#define H_date

#include <string>
#include <boost/json.hpp>

using namespace std;
using json = boost::json::value;

class Date {
 
public:

  static string toISODate(long y);
  static long now();

};

#endif // H_date
