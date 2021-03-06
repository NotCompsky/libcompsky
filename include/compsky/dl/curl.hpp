#pragma once

#ifdef USE_LIBCURL

#include <compsky/macros/str_parsing.hpp> // for SKIP_TO_HEADER
#include <curl/curl.h>
#include <cstring> // for memccpy
#include <string_view>
#include <compsky/asciify/asciify.hpp>


namespace compsky {
namespace dl {


template<size_t sz>
class CurlSList {
  public:
	curl_slist nodes[sz];
	std::size_t size;
	
	template<size_t i,  typename... Args>
	void set_headers(){}
	
	template<size_t i = 0,  typename... Args>
	void set_headers(const char* header,  Args... args){
		++this->size;
		if constexpr(sizeof...(Args) == 0)
			this->nodes[i].next = nullptr;
		else
			this->nodes[i].next = &this->nodes[i+1];
		this->nodes[i].data = const_cast<char*>(header);
		this->set_headers<i+1>(args...);
	}
	
	void append_header(const char* header){
		this->nodes[this->size++].data = const_cast<char*>(header);
	}
	
	CurlSList()
	: size(0)
	{}
	
	template<typename... Args>
	CurlSList(Args... args)
	: size(0)
	{
		this->set_headers<0>(args...);
	}
};


template<std::size_t n_headers = 15>
class Curl {
  public:
	CURL* const obj;
	CurlSList<n_headers> headers;
	
	template<typename... Args>
	Curl(Args&&... args)
	: obj(curl_easy_init())
	{
		this->init(args...);
	}
	
	void init(){}
	
	template<typename Auto1,  typename Auto2,  typename... Args>
	void init(Auto1 opt,  Auto2 value,  Args&&... args){
		this->set_opt(opt, value);
		this->init(args...);
	}
	
	~Curl(){
		curl_easy_cleanup(this->obj);
	}
	
	template<typename Auto1,  typename Auto2>
	void set_opt(Auto1 opt,  Auto2 value){
		curl_easy_setopt(this->obj, opt, value);
	}
	
	void set_url(const char* const url){
		this->set_opt(CURLOPT_URL, url);
	}
	
	bool perform() const {
		return (curl_easy_perform(this->obj) != CURLE_OK);
	}
	
	bool perform(const char* const url){
		this->set_url(url);
		return this->perform();
	}
	
	bool perform(const std::string_view v){
		char url[4000];
		compsky::asciify::asciify(url, v, '\0');
		return this->perform(url);
	}
	
	bool copy_mimetype(char* buf,  const std::size_t max_bytes){
		char* _mimetype = nullptr;
		const auto curl_rc2 = curl_easy_getinfo(this->obj, CURLINFO_CONTENT_TYPE, &_mimetype);
		if (_mimetype and not curl_rc2)
			memccpy(buf, _mimetype, 0, max_bytes);
		return (_mimetype and not curl_rc2);
	}
	
	bool copy_headers(const char* user_headers,  char* user_agent_buf){
		const char* const user_agent = SKIP_TO_HEADER(12,"User-Agent: ")(user_headers);
		if (user_agent == nullptr)
			return true;
		memccpy(user_agent_buf,  user_agent - 11,  '\r',  1000);
		compsky::str::replace_first_instance_of(user_agent_buf, '\r', '\0');
		
		this->headers.set_headers(
			user_agent_buf,
			"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8",
			"Accept-Language: en-GB,en;q=0.5",
			"Accept-Encoding: gzip, deflate, br",
			"Connection: keep-alive",
			"Upgrade-Insecure-Requests: 1",
			"DNT: 1",
			"Pragma: no-cache",
			"Cache-Control: no-cache",
			"TE: Trailers"
		);
		this->set_opt(CURLOPT_HTTPHEADER, this->headers.nodes[1]);
		return false;
	}
	
	void append_header(const char* header){
		this->headers.append_header(header);
	}
};

} // namespace dl
} // namespace compsky

#endif // USE_LIBCURL
