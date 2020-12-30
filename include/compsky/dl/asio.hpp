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
void split_url(const char* url,  bool& is_https,  std::string_view& host,  std::string_view& path){
	const char* url_itr = url - 1;
	is_https = (IS_STR_EQL(url_itr,5,"https"));
	// url_itr is how either past https (if it is https://...) or past http (if it is http://)
	const char* const host_start = url_itr + 1 + 3;
	host = std::string_view(host_start,  compsky::str::count_until(host_start, '/'));
	path = std::string_view(host.end(),  strlen(host_start + host.size()));
}

inline
void split_url(std::string_view const url,  bool& is_https,  std::string_view& host,  std::string_view& path){
	const char* url_itr = url.data() - 1;
	is_https = (IS_STR_EQL(url_itr,5,"https"));
	// url_itr is how either past https (if it is https://...) or past http (if it is http://)
	const char* const host_start = url_itr + 1 + 3;
	host = std::string_view(host_start,  compsky::str::count_until(host_start, '/'));
	path = std::string_view(host.data() + host.size(),  url.size() - utils::ptrdiff(host.data(), url.data()));
}

template<typename... Args>
void split_url(std::tuple<Args...> url,  bool& is_https,  std::string_view& host,  std::string_view& path){
	// WARNING: Assumes that the first parameter of url is a string beginning https://HOST/
	return split_url(std::get<0>(url), is_https, host, path);
}

enum ResponseCode {
	invalid,
	ok,
	redirect,
	partial
};

inline
ResponseCode get_response_code(const char* response){
	switch(response[0]){
		case '2':
			switch(response[1]){
				case '0':
					switch(response[2]){
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
			switch(response[1]){
				case '0':
					switch(response[2]){
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


template<typename Orig>
class CookiesAndAppendedCookies : public compsky::asciify::_detail::ExtendWithBuf<CookiesAndAppendedCookies<Orig>, Orig> {
 public:
	const char* separator() const {
		return "; ";
	}
	template<typename A,  typename B>
	CookiesAndAppendedCookies(const A& a,  const B& b)
	: compsky::asciify::_detail::ExtendWithBuf<CookiesAndAppendedCookies<Orig>, Orig>::ExtendWithBuf(a, b)
	{}
};


template<typename Orig,  typename T>
struct StructToDetTyp {
	typedef CookiesAndAppendedCookies<Orig> typ;
};
template<typename Orig,  typename T>
struct StructToDetTyp<CookiesAndAppendedCookies<Orig>, T> {
	typedef CookiesAndAppendedCookies<Orig> typ;
};



} // namespace _detail


template<typename Path,  typename Cookies,  typename Mimetype,  typename... RequestStrArgs>
size_t dl(char* const req_str_buf,  const size_t req_str_buf_sz,  char*& dst_buf,  const char* const dst_pth,  Mimetype mimetype,  const char* const method,  bool is_https,  std::string_view host,  Path path,  Cookies cookies,  RequestStrArgs&&... request_str_args){
	// WARNING: This assumes that no response will be larger than the dst_buf - which should be at least 10 MiB
	
	// dst_buf_orig is used to temporarily construct the request string, and stores the response string
	
	constexpr bool is_copying_to_file = std::is_same<char**, Mimetype>::value;
	
	char* dst_buf_orig = dst_buf;
	
	const std::string_view port = is_https ? "443" : "80";
	
	boost::asio::io_service io_service;
	boost::system::error_code err;
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = boost::asio::ip::tcp::resolver(io_service).resolve(host, port, err);
	
	if (unlikely(err))
		return 0;
	
	char* req_str_itr = req_str_buf;
	compsky::asciify::asciify(req_str_itr, method, ' ', path, " HTTP/1.1");
	if constexpr(not std::is_same<std::nullptr_t, Cookies>::value)
		compsky::asciify::asciify(req_str_itr, "\r\nCookies: ", cookies);
	compsky::asciify::asciify(req_str_itr, "\r\n", request_str_args...);
	
	printf("\n\nreq_str_buf first 500 chars: %.500s\n", req_str_buf);
	
	boost::asio::const_buffer  request(req_str_buf,  utils::ptrdiff(req_str_itr, req_str_buf));
	
	const std::size_t max_bytes_to_read = req_str_buf_sz - 1;
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
	
	
	printf("\n\ndst_buf_orig first 1000 chars: %.1000s\n", dst_buf_orig);
	
	const char* response_itr = dst_buf_orig - 1;
	if (unlikely(not IS_STR_EQL(response_itr,9,"HTTP/1.1 ")))
		return 0;
	
	size_t byte_offset = 0;
	size_t n_bytes = 0;
	
	switch(_detail::get_response_code(response_itr + 1)){
		case _detail::ResponseCode::ok:
			break;
		case _detail::ResponseCode::redirect: {
			const char* _http_itr = response_itr;
			typename compsky::dl::asio::_detail::StructToDetTyp<Cookies, Cookies>::typ _cookies(cookies, req_str_buf);
			while(true){
				const std::string_view set_cookie = STRING_VIEW_FROM_UP_TO(14, "\r\nset-cookie: ")(_http_itr, ';');
				printf("set_cookie == %.*s\n", (int)set_cookie.size(), set_cookie.data()); fflush(stdout);
				if (set_cookie == utils::nullstrview)
					break;
				_cookies.append(set_cookie);
				_http_itr = set_cookie.data() + 1;
			}
			printf("Following redirect\n"); fflush(stdout);
			const std::string_view redirect_url = STRING_VIEW_FROM_UP_TO(12, "\r\nLocation: ")(response_itr, '\r');
			std::string_view new_path;
			if (redirect_url.at(0) == '/')
				new_path = redirect_url;
			else
				_detail::split_url(redirect_url, is_https, host, new_path);
			return (unlikely(redirect_url == compsky::utils::nullstrview)) ? 0 : dl(_cookies.after(), req_str_buf_sz - _cookies.size(), dst_buf, dst_pth, mimetype, method, is_https, host, new_path, _cookies, request_str_args...);
		}
		case _detail::ResponseCode::partial: {
			if (SKIP_TO_HEADER(34, "Content-Type: multipart/byteranges")(response_itr) != nullptr)
				// Is a multipart response
				return 0; // TODO: Implement
			const char* const range_from = SKIP_TO_HEADER(21, "Content-Range: bytes ")(response_itr);
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
