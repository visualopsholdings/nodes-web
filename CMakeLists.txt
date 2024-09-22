#   CMakeLists.txt
#
#   Author: Paul Hamilton (paul@visualops.com)
#   Date: 21-May-2024
#
#   Input file for nodes web driver for Visual Ops
#
#   Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
#   
#   https://github.com/visualopsholdings/nodes-web

cmake_minimum_required(VERSION 3.5)

project (nodes-web)
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

# fetch from other repos.

include(FetchContent)

FetchContent_Declare(
  nodes-test
  GIT_REPOSITORY https://github.com/visualopsholdings/nodes-test.git
  GIT_TAG main
)
FetchContent_GetProperties(nodes-test)

if(NOT nodes-test_POPULATED)
  message(STATUS "Fetching nodes-test...")
  FetchContent_Populate(nodes-test)
endif()

set(LIBS zmq)
set(BOOSTLIBS ${Boost_LOG_LIBRARY} ${Boost_JSON_LIBRARY})
set(UTILS
  src/json.cpp
  src/cookie.cpp
)
set(HANDLERS
  src/handlers/get/rawusers.cpp
  src/handlers/get/streams.cpp
  src/handlers/get/conversation.cpp
  src/handlers/get/login.cpp
  src/handlers/get/logout.cpp
  src/handlers/get/me.cpp
  src/handlers/get/stream.cpp
  src/handlers/get/streampolicyusers.cpp
  src/handlers/get/streampolicygroups.cpp
  src/handlers/get/user.cpp
  src/handlers/post/login.cpp
  src/handlers/post/typing.cpp
  src/handlers/get/infos.cpp
  src/handlers/get/rawsites.cpp
  src/handlers/get/sites.cpp
  src/handlers/post/newuser.cpp
  src/handlers/post/users.cpp
  src/handlers/get/websocket.cpp
  src/handlers/sendWS.cpp
  src/handlers/get/rawgroups.cpp
  src/handlers/get/rawgroup.cpp
  src/handlers/get/rawgroupusers.cpp
  src/handlers/get/users.cpp
  src/handlers/post/groupusers.cpp
  src/handlers/delete/groupusers.cpp
  src/handlers/get/rawuser.cpp
  src/handlers/get/groups.cpp
  src/handlers/get/groupusers.cpp
  src/handlers/get/rawstreams.cpp
  src/handlers/get/rawstream.cpp
  src/handlers/get/rawstreampolicy.cpp
  src/handlers/get/rawgrouppolicy.cpp
  src/handlers/delete/stream.cpp
  src/handlers/delete/group.cpp
  src/handlers/get/streamsharelink.cpp
  src/handlers/get/canreg.cpp
  src/handlers/sendIdeaWS.cpp
)

include_directories(include)

add_executable(nodes-web src/nodes-web.cpp ${UTILS} ${HANDLERS} src/session.cpp src/server.cpp  src/zmqclient.cpp)
  target_link_libraries(nodes-web ${Boost_PROGRAM_OPTIONS_LIBRARY} ${BOOSTLIBS} zmq $ENV{RESTINIO_HOME}/dev/cmake_build/lib/libfmtd.a $ENV{RESTINIO_HOME}/dev/cmake_build/lib/libllhttp.a)
  target_include_directories(nodes-web PUBLIC $ENV{RESTINIO_HOME}/dev $ENV{RESTINIO_HOME}/dev/nodejs/llhttp/include $ENV{RESTINIO_HOME}/dev/expected-lite/include $ENV{RESTINIO_HOME}/dev/fmt/include)
  target_compile_definitions(nodes-web  PUBLIC RESTINIO_USE_BOOST_ASIO=1)

add_executable(TestSession test/testsession.cpp src/session.cpp ${UTILS})
  target_link_libraries(TestSession ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${BOOSTLIBS})

add_test(TestSession TestSession)

add_executable(TestJson test/testjson.cpp src/json.cpp)
  target_link_libraries(TestJson ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${BOOSTLIBS})

add_test(TestJson TestJson)

add_executable(TestCookie test/testcookie.cpp src/cookie.cpp)
  target_link_libraries(TestCookie ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${BOOSTLIBS})

add_test(TestCookie TestCookie)
