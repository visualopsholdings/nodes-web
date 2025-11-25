/*
  postmediaupload.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Nov-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes-web
*/

#include "server.hpp"
#include "session.hpp"
#include "etag.hpp"
#include "dict.hpp"

#include <boost/log/trivial.hpp>
#include <restinio/router/express.hpp>
#include <restinio/core.hpp>
#include <restinio/helpers/file_upload.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace restinio::file_upload;

namespace nodes {

status_t postmediaupload(Server *server, const req_t& req, params_t params) {

  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }

  const auto id = restinio::cast_to<string>(params["id"]);
  if (id == "undefined") {
    return server->returnEmptyObj(req, ETag::none());
  }
  BOOST_LOG_TRIVIAL(trace) << "idea " << id;
  const auto qp = restinio::parse_query(req->header().query());
  if (!qp.has("type")) {
    return server->fatal(req, "missing type");
  }
  string type = restinio::cast_to<string>(qp["type"]);
  if (type != "IMAGE") {
    return server->fatal(req, "can only deal with images");
  }

  vector<tuple<string, string> > files;
  
	const auto enumeration_result = enumerate_parts_with_files(*req,
			[&files, server](const part_description_t & part) {
        BOOST_LOG_TRIVIAL(trace) << part.name;
				if (part.name == "files") {
					if (!part.filename) {
            BOOST_LOG_TRIVIAL(error) << "files part missing filename";
				    return handling_result_t::terminate_enumeration;
					}
					
					// create a UUID for the file.
          auto uuid = boost::uuids::to_string(boost::uuids::random_generator()());

          // remember the filesnames and UUIDs
          files.push_back(tuple<string, string>(uuid, *part.filename));

					// write this file.
          std::ofstream dest_file;
          dest_file.exceptions(std::ofstream::failbit);
          stringstream ss;
          ss << server->_mediaPath;
          ss << "/";
          ss << uuid;
          dest_file.open(ss.str());
          dest_file.write(part.body.data(), part.body.size());

				}
        else if (part.name == "idea") {
          BOOST_LOG_TRIVIAL(trace) << "idea " << part.body;
        }
        else if (part.name == "type") {
          BOOST_LOG_TRIVIAL(trace) << "type " << part.body;
        }
        
        // go through everything.
        return handling_result_t::continue_enumeration;
			} );

	if (!enumeration_result) {
    BOOST_LOG_TRIVIAL(trace) << *enumeration_result;
	  return server->fatal(req, "file content not found!");
	}

  auto images = DictV();
  for (auto i: files) {
    images.push_back(dictO({
      { "uuid", get<0>(i) },
      { "filename", get<1>(i) }
    }));
  }
  
  auto msg = dictO({
    { "type", "setobject" },
    { "objtype", "idea" },
    { "id", id },
    { "images", images }
  });
  
  server->send(msg);
  auto j = server->receive();
  
//  BOOST_LOG_TRIVIAL(trace) << "received " << Dict::toString(j);
  
  auto resp = server->checkErrors(req, j, type);
  if (resp) {
    return resp.value();
  }
  
  return server->returnEmptyObj(req, ETag::none());

}

};
