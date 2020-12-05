#pragma once

#include <atomic>
#include <thread>
#include <boost/asio.hpp>

#include "log.hpp"
#include "http_acceptor.hpp"


namespace compsky {
namespace server {


extern
std::vector<std::string> banned_client_addrs;


namespace _detail {
	bool is_client_banned(const std::string& client_addr){
		return (std::find(banned_client_addrs.begin(), banned_client_addrs.end(), client_addr) == banned_client_addrs.end());
	}
}


template<unsigned n_threads,  class Service>
class HttpServer {
  public:
	HttpServer()
	: is_stopped(false)
	{}
	
	void start(const unsigned short port_id){
		this->thread.reset(new std::thread([this, port_id]() {
			this->start_acceptor(port_id);
		}));
		log("Server started on port ", port_id);
	}
	void stop(){
		this->is_stopped.store(true);
		this->thread->join();
	}

  private:
	void start_acceptor(const unsigned short port_id){
		HttpAcceptor<Service> acceptor(port_id);
		while(not this->is_stopped.load()) {
			acceptor.start();
		}
	}
	std::unique_ptr<std::thread> thread;
	std::atomic<bool> is_stopped;
};



}
}
