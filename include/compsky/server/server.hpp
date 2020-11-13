#pragma once

#include "defines.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>


namespace compsky {
namespace server {


extern
std::vector<std::string> banned_client_addrs;


namespace _detail {
	bool is_client_banned(const std::string& client_addr){
		return (std::find(banned_client_addrs.begin(), banned_client_addrs.end(), client_addr) == banned_client_addrs.end());
	}
}


template<unsigned n_threads,  typename ConnectionHandler>
class Server {
	typedef std::shared_ptr<ConnectionHandler> SharedHandler;
 private:
	std::vector<std::thread> threads;
	asio::io_service service;
	asio::ip::tcp::acceptor acceptor;
	
	void handle_new_connection(SharedHandler handler,  boost::system::error_code const& err){
		if (unlikely(err))
			return;
		handler->start();
		auto handler2 = std::make_shared<ConnectionHandler>(this->service);
		this->acceptor.async_accept(handler2->get_socket(), [=](auto err){ this->handle_new_connection(handler2, err); });
	}
 public:
	Server()
	: acceptor(this->service)
	{}
	
	void start(const unsigned port){
		fprintf(stderr,  "Starting\n");  fflush(stderr);
		
		auto handler = std::make_shared<ConnectionHandler>(this->service);
		
		fprintf(stderr,  "Made shared handler\n");  fflush(stderr);
		
		asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);
		this->acceptor.open(endpoint.protocol());
		this->acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
		this->acceptor.bind(endpoint);
		this->acceptor.listen();
		
		fprintf(stderr,  "Listening\n");  fflush(stderr);
		
		this->acceptor.async_accept(handler->get_socket(), [=](auto err){
			fprintf(stderr,  "Server::start::lambda remote addr == %s\n",  handler->get_socket().remote_endpoint().address().to_string()); fflush(stderr);
			this->handle_new_connection(handler, err); 
		});
		
		fprintf(stderr,  "Accepting\n");  fflush(stderr);
		
		for (unsigned i = 0;  i < n_threads;  ++i){
			this->threads.emplace_back([=]{ this->service.run(); });
		}
		
		fprintf(stderr,  "Created threads\n");  fflush(stderr);
	}
};

}
}
