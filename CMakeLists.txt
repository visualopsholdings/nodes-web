#   CMakeLists.txt
#
#   Author: Paul Hamilton (paul@visualops.com)
#   Date: 21-May-2024
#
#   Input file for zchttp driver for Visual Ops
#
#   Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
#   
#   https://github.com/visualopsholdings/zchttp

cmake_minimum_required(VERSION 3.5)

project (czhttp)
  find_package(Boost 1.85.0 COMPONENTS program_options filesystem unit_test_framework chrono thread log_setup log json REQUIRED)
  find_package(cppzmq REQUIRED)
  add_definitions(-DBOOST_ALL_DYN_LINK) 
  enable_testing()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

if (UNIX AND NOT APPLE)
  add_definitions(-funwind-tables) 
endif ()
if (APPLE)
  include_directories(/usr/local/include)
  LINK_DIRECTORIES(/opt/homebrew/Cellar/zeromq/4.3.5_1/lib)
endif ()

set(LIBS zmq)
set(BOOSTLIBS ${Boost_LOG_LIBRARY} ${Boost_JSON_LIBRARY})
set(UTILS
  src/json.cpp
  src/cookie.cpp
)

include_directories(include)

add_executable(zchttp src/zchttp.cpp ${UTILS} src/session.cpp)
  target_link_libraries(zchttp ${Boost_PROGRAM_OPTIONS_LIBRARY} ${BOOSTLIBS} zmq $ENV{RESTINIO_HOME}/dev/cmake_build/lib/libfmtd.a $ENV{RESTINIO_HOME}/dev/cmake_build/lib/libllhttp.a)
  target_include_directories(zchttp PUBLIC $ENV{RESTINIO_HOME}/dev $ENV{RESTINIO_HOME}/dev/nodejs/llhttp/include $ENV{RESTINIO_HOME}/dev/expected-lite/include $ENV{RESTINIO_HOME}/dev/fmt/include)
  target_compile_definitions(zchttp  PUBLIC RESTINIO_USE_BOOST_ASIO=1)

add_executable(TestSession test/testsession.cpp src/session.cpp ${UTILS})
  target_link_libraries(TestSession ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${BOOSTLIBS})

add_test(TestSession TestSession)

add_executable(TestJson test/testjson.cpp src/json.cpp)
  target_link_libraries(TestJson ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${BOOSTLIBS})

add_test(TestJson TestJson)

add_executable(TestCookie test/testcookie.cpp src/cookie.cpp)
  target_link_libraries(TestCookie ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestCookie TestCookie)
