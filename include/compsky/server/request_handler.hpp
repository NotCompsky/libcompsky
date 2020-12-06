#pragma once

#include <vector>
#include <boost/noncopyable.hpp>

namespace compsky {
namespace server {

class request_handler : private boost::noncopyable {
public:
	void handle_request(boost::array<char, 8192>& req_buffer,  const size_t n_bytes_of_first_req_buffer,  std::vector<boost::asio::const_buffer>& response_buffers);
	// If n_bytes_of_first_req_buffer == sizeof(req_buffer), then possibly need to perform more reads
};

} // namespace server
} // namespace compsky
