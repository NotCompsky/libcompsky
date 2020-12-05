#pragma once

#include <iostream>
#include <string>
//#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string_view>


namespace compsky {
namespace server {


typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& SSLStream;


namespace _f {
	constexpr compsky::asciify::flag::UntilNullOr until_null_or;
}


template<class Derived>
class HttpService {
  public:
	HttpService()
	{}
	
    void handle_request(SSLStream& ssl_stream){
		try{
			//perform TLS handshake
			ssl_stream.handshake(boost::asio::ssl::stream_base::server);
			
			const size_t n_bytes_read = boost::asio::read(
				ssl_stream,
				boost::asio::buffer(this->request_buffer, sizeof(this->request_buffer)),
				boost::asio::transfer_exactly(sizeof(this->request_buffer))
			); // Stops on EOF, buffer full, or when n bytes reached
			
			char ip_buf[40];
			log("Handling client: ", this->get_ip(ip_buf, ssl_stream), n_bytes_read, " bytes\n", _f::until_null_or, '\n', this->request_buffer);
			// TODO: Parse the string request_line
			
			static_cast<Derived*>(this)->process_request(n_bytes_read, ssl_stream);
		} catch(const boost::system::system_error& ec){
			log("System error ", ec.code().value(), " occurred: ", ec.what());
		}
	}
	
  protected:
	char request_buffer[4096];
	std::string_view get_ip(char* buf,  SSLStream& ssl_stream) const {
		boost::asio::ip::tcp::endpoint ep = ssl_stream.lowest_layer().remote_endpoint();
		boost::asio::ip::address addr = ep.address();
		const std::string str = addr.to_string();
		memcpy(buf, str.c_str(), str.size());
		return std::string_view(buf, str.size());
	}
	// To overwrite: void process_request(const size_t n_bytes_read,  SSLStream& ssl_stream);
    void send_response(SSLStream ssl_stream){
		if(m_ResourceSizeInBytes > 0){
			this->response_buffers.push_back(boost::asio::buffer(this->response_buffer.get(), m_ResourceSizeInBytes));
		}
		boost::asio::write(ssl_stream, this->response_buffers);
		m_ResourceSizeInBytes = 0;
	}
	
  private:
	std::vector<boost::asio::const_buffer> response_buffers;
	std::unique_ptr<char[]> response_buffer;
	std::size_t m_ResourceSizeInBytes;
};


}
}
