/*
  getcontent.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 27-Nov-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "json.hpp"
#include "etag.hpp"

#include <restinio/core.hpp>
#include <boost/log/trivial.hpp>
#include <restinio/sendfile.hpp>

namespace fs = std::filesystem;

namespace nodes {

status_t getcontent(Server *server, const req_t& req, params_t params)
{
  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }

  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyObj(req, ETag::none());
  }
  auto msg = dictO({
    { "type", "object" },
    { "objtype", "media" },
    { "me", session.value()->userid() },
    { "id", id }
  });
  auto j = server->callNodes(msg);

  auto resp = server->checkErrors(req, j, "getmedia");
  if (resp) {
    return resp.value();
  }

  auto media = Dict(j).object("media");
  auto uuid = media.object("uuid").string();
  auto filename = media.object("name").string();
  
//  BOOST_LOG_TRIVIAL(trace) << "filename " << name;

  stringstream ss;
  ss << server->_mediaPath;
  ss << "/";
  ss << uuid;
  auto sf = restinio::sendfile(ss.str());

  auto modified_at = restinio::make_date_field_value(sf.meta().last_modified_at());
  auto expires_at = restinio::make_date_field_value(std::chrono::system_clock::now() + std::chrono::hours(24 * 7));

  auto response = req->create_response();

  response.append_header_date_field();
  response.append_header(restinio::http_field::last_modified, std::move(modified_at));
  response.append_header(restinio::http_field::expires, std::move(expires_at));
  
  fs::path p = filename;
  if (p.extension() == ".jpeg" || p.extension() == ".jpg") {
    response.append_header(restinio::http_field::content_type, "image/jpeg");
  }
  else if (p.extension() == ".png") {
    response.append_header(restinio::http_field::content_type, "image/png");
  }
  else if (p.extension() == ".heic") {
    response.append_header(restinio::http_field::content_type, "image/heic");
  }

  response.set_body(std::move(sf));
  
  return response.done();

}

};
