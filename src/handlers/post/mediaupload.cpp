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

using namespace restinio::file_upload;

namespace nodes {

status_t postmediaupload(Server *server, const req_t& req, params_t params) {

  auto session = server->getSession(req);
  if (!session) {
    return server->unauthorised(req);
  }

	const auto enumeration_result = enumerate_parts_with_files(*req,
			[](const part_description_t & part) {
//        BOOST_LOG_TRIVIAL(trace) << "got part " << part.name;
				if (part.name == "file") {
					if (part.filename) {
					
//            BOOST_LOG_TRIVIAL(trace) << *part.filename;
            
            std::ofstream dest_file;
            dest_file.exceptions(std::ofstream::failbit);
            stringstream ss;
            ss << "/Users/paul/Dev/nodes-web/private/media/";
            ss << *part.filename;
            dest_file.open(ss.str());
            dest_file.write(part.body.data(), part.body.size());

						// There is no need to handle other parts.
						return handling_result_t::stop_enumeration;
					}
				}

				// We expect only one part with name 'file'.
				// So if that part is not found yet there is some error
				// and there is no need to continue.
				return handling_result_t::terminate_enumeration;
			} );

	if (!enumeration_result || 1u != *enumeration_result) {
    BOOST_LOG_TRIVIAL(trace) << *enumeration_result;
	  return server->fatal(req, "file content not found!");
	}

  return server->returnEmptyObj(req, ETag::none());

}

};
