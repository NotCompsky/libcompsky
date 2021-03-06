/*
Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com) under the Boost v1 license:

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <compsky/macros/likely.hpp>
#include "connection.hpp"


namespace compsky {
namespace server {

template<size_t thread_pool_size_,  size_t req_buffer_sz,  class RequestHandler>
class Server : private boost::noncopyable {
 private:
	boost::asio::io_context io_context_;
	boost::asio::signal_set signals_;
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::shared_ptr<Connection<req_buffer_sz, RequestHandler>> new_connection_;
 public:
	Server(const unsigned port_id)
	: signals_(io_context_)
	, acceptor_(io_context_)
	, new_connection_()
	{
		// Register to handle the signals that indicate when the server should exit.
		// It is safe to register for the same signal multiple times in a program,
		// provided all registration for the specified signal is made through Asio.
		signals_.add(SIGINT);
		signals_.add(SIGTERM);
	  #ifdef SIGQUIT
		signals_.add(SIGQUIT);
	  #endif
		signals_.async_wait(boost::bind(&Server::handle_stop, this));
		
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("0.0.0.0"), port_id);
		acceptor_.open(endpoint.protocol());
		acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor_.bind(endpoint);
		acceptor_.listen();

		start_accept();
	}
	
	void run(){
		std::array<boost::shared_ptr<boost::thread>, thread_pool_size_> threads;
		for (auto i = 0;  i < thread_pool_size_;  ++i){
			boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_context::run, &io_context_)));
			threads[i] = thread;
		}
		for (auto i = 0;  i < thread_pool_size_;  ++i)
			threads[i]->join();
	}

 private:
	/// Initiate an asynchronous accept operation.
	void start_accept(){
		new_connection_.reset(new Connection<req_buffer_sz, RequestHandler>(io_context_));
		acceptor_.async_accept(
			new_connection_->socket(),
			boost::bind(
				&Server::handle_accept,
				this,
				boost::asio::placeholders::error
			)
		);
	}
	
	void handle_accept(const boost::system::error_code& e){
		if (likely(not e))
			new_connection_->start();
		start_accept();
	}
	
	void handle_stop(){
		io_context_.stop();
	}
};

} // namespace server
} // namespace compsky
