#pragma once

//#include <memory>
#include <atomic>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "http_service.hpp"


namespace compsky {
namespace server {


template<class Service>
class HttpAcceptor {
  public:
	HttpAcceptor(boost::asio::io_service& _service,  const unsigned short port_id)
	: service(_service)
	, acceptor(_service,  boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port_id))
	, is_stopped(false)
	, ssl_ctx(boost::asio::ssl::context::sslv23_server)
	{
		this->ssl_ctx.set_options(
			  boost::asio::ssl::context::default_workarounds
			| boost::asio::ssl::context::no_sslv2
			| boost::asio::ssl::context::single_dh_use
		);
		this->ssl_ctx.use_certificate_chain_file("server.cert");
		this->ssl_ctx.use_private_key_file("server.key", boost::asio::ssl::context::pem);
	}
	void start(){
		this->acceptor.listen();
		boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_stream(this->service, this->ssl_ctx);
		
		this->acceptor.accept(ssl_stream.lowest_layer());
		
		HttpService<Service> _service;
		_service.handle_request(ssl_stream);
	}
	void stop(){
		this->is_stopped.store(true);
	}

  private:
	boost::asio::io_service service;
	boost::asio::ip::tcp::acceptor acceptor;
	std::atomic<bool> is_stopped;
	boost::asio::ssl::context ssl_ctx;
};


}
}
