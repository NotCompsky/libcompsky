#pragma once

#include <compsky/macros/str_parsing.hpp>
#include <compsky/str/parse.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <compsky/utils/ptrdiff.hpp>


namespace compsky {
namespace dl {
namespace asio {


namespace _detail {

inline
const std::string_view get_url_conn_data(const char* url,  bool& is_https){
	const char* url_itr = url - 1;
	is_https = (IS_STR_EQL(url_itr,5,"https"));
	// url_itr is how either past https (if it is https://...) or past http (if it is http://)
	const char* const host_start = url_itr + 1 + 3;
	return std::string_view(host_start,  compsky::str::count_until(host_start, '/'));
}

inline
const std::string_view get_url_conn_data(std::string_view const url,  bool& is_https){
	return get_url_conn_data(url.data(), is_https);
}

enum ResponseCode {
	invalid,
	ok,
	redirect,
	partial
};

inline
ResponseCode get_response_code(const char* response){
	switch(response_itr[1]){
		case '2':
			switch(response_itr[2]){
				case '0':
					switch(response_itr[3]){
						case '0':
							// OK
							return ResponseCode::ok;
						case '6':
							// TODO: Deal with partial response
							return ResponseCode::partial;
						default:
							return ResponseCode::invalid;
					}
				default:
					return ResponseCode::invalid;
			}
		case '3':
			switch(response_itr[2]){
				case '0':
					switch(response_itr[3]){
						case '1':
						case '2':
						case '3':
						case '4':
						case '7':
						case '8':
							return ResponseCode::redirect;
						default:
							return ResponseCode::invalid;
					}
				default:
					return ResponseCode::invalid;
			}
		default:
			return ResponseCode::invalid;
	}
}

} // namespace _detail


template<typename Url,  typename Mimetype>
size_t dl(Url const url,  const std::string_view request_str,  char*& dst_buf,  const char* const dst_pth,  Mimetype mimetype){
	// dst_buf_orig is used to temporarily construct the request string, and stores the response string
	
	constexpr bool is_copying_to_file = std::is_same<char**, Mimetype>::value;
	
	char* dst_buf_orig = dst_buf;
	
	bool is_https;
	const std::string_view host = _detail::get_url_conn_data(url, is_https);
	const std::string_view port = is_https ? "443" : "80";
	
	boost::asio::io_service io_service;
	boost::system::error_code err;
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = boost::asio::ip::tcp::resolver(io_service).resolve(host, port, err);
	
	if (unlikely(err))
		return 0;
	
	boost::asio::const_buffer request(request_str.data(), request_str.size());
	
	constexpr size_t max_bytes_to_read = HANDLER_BUF_SZ - 1;
	size_t n_bytes_read;
	
	if (is_https){
		boost::asio::ssl::context ssl_ctx(boost::asio::ssl::context::method::sslv23_client);
		boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket(io_service, ssl_ctx);
		boost::asio::connect(socket.lowest_layer(), endpoint_iterator);
		socket.handshake(boost::asio::ssl::stream_base::handshake_type::client);
		// TODO: Verify certificates
		boost::asio::write(socket, request);
		n_bytes_read = boost::asio::read(
			socket,
			boost::asio::mutable_buffer(dst_buf_orig, max_bytes_to_read),
				// WARNING: Should deduct some amount of bytes, as we have probably filled some content already
			err
		);
	} else {
		boost::asio::ip::tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);
		boost::asio::write(socket, request);
		n_bytes_read = boost::asio::read(
			socket,
			boost::asio::mutable_buffer(dst_buf_orig, max_bytes_to_read),
			err
		);
	}
	if (unlikely(not err)){ // if (unlikely(n_bytes_read == max_bytes_to_read)){}
		// This means it didn't encounter an EOF - i.e. n_bytes_read == max_bytes_to_read
		// boost::asio doesn't actually have a function overload involving a non-dynamically allocated buffer for a read function where you read until EOF *without* an error
		if constexpr (not is_copying_to_file)
			// Do not attempt to enlarge the buffer
			return 0;
	}
	
	
	printf("dst_buf_orig first 1000 chars: %.1000s\n", dst_buf_orig);
	
	const char* response_itr = dst_buf_orig - 1;
	if (unlikely(not IS_STR_EQL(response_itr,9,"HTTP/1.1 ")))
		return 0;
	
	size_t byte_offset = 0;
	size_t n_bytes = 0;
	
	switch(_detail::get_response_code(response_itr)){
		case _detail::ResponseCode::ok:
			break;
		case _detail::ResponseCode::redirect: {
			const std::string_view redirect_url = STRING_VIEW_FROM_UP_TO(12, "\r\nLocation: ")(response_itr, '\r');
			return dl(redirect_url, request_str, dst_buf, dst_pth, mimetype);
		}
		case _detail::ResponseCode::partial: {
			if (SKIP_TO_HEADER("Content-Type: multipart/byteranges")(response_itr) != nullptr)
				// Is a multipart response
				return 0; // TODO: Implement
			const char* const range_from = SKIP_TO_HEADER("Content-Range: bytes ")(response_itr);
			size_t to;
			if (unlikely(compsky::http::header::get_range_from(range_from, byte_offset, to) != compsky::http::header::valid))
				return 0;
			if constexpr(not is_copying_to_file){
				if (unlikely(to >= HANDLER_BUF_SZ))
					// Would overflow our buffer
					return 0;
			}
			n_bytes = to - byte_offset;
			break;
		}
		case _detail::ResponseCode::invalid:
			return 0;
	}
	
	dst_buf_orig[n_bytes_read] = 0;
	
	char* const start_of_content = const_cast<char*>(SKIP_TO_HTTP_CONTENT(dst_buf_orig));
	n_bytes = n_bytes_read - utils::ptrdiff(start_of_content, dst_buf_orig);
	if constexpr (not is_copying_to_file)
		dst_buf = start_of_content;
	
	if (unlikely(start_of_content == nullptr))
		// Who on Earth has 11 megabytes' worth of headers???
		return 0;
	
	if constexpr (is_copying_to_file){
		compsky::os::WriteOnlyFile f(dst_pth);
		if (unlikely(f.is_null()))
			return 0;
		f.write_from_buffer_at_offset(dst_buf_orig, n_bytes, byte_offset);
	}
	
	return n_bytes;
}


} // namespace asio
} // namespace dl
} // namespace compsky
