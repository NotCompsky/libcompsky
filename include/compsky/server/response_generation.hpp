#pragma once

#include "defines.hpp"
#include "static_response.hpp"
#include "jsonify.hpp"
#include "cache.hpp"
#include <compsky/mysql/query.hpp>


namespace compsky {
namespace server {


extern
std::vector<std::string> banned_client_addrs;


namespace _r {
	namespace flag {
		constexpr static const Arr arr;
		constexpr static const Dict dict;
		
		constexpr static QuoteAndJSONEscape quote_and_json_escape;
		constexpr static QuoteAndEscape quote_and_escape;
		constexpr static QuoteNoEscape quote_no_escape;
		constexpr static NoQuote no_quote;
	};
}


class ResponseGeneration {
 protected:
	char* buf;
	char* itr;
	
	MYSQL_RES* res[2];
	MYSQL_ROW row;
	
	constexpr
	uintptr_t buf_indx(){
		return (uintptr_t)this->itr - (uintptr_t)this->buf;
	}
	
	constexpr
	void reset_buf_index(){
		this->itr = this->buf;
	}
	
	inline
	char last_char_in_buf(){
		return *(this->itr - 1);
	}
	
	constexpr
	void move_itr_to_trailing_null(){
		while(*this->itr != 0)
			++this->itr;
	}
	
	template<typename... Args>
	void asciify(Args... args){
		compsky::asciify::asciify(this->itr,  args...);
	};
	
	template<size_t i = 0,  typename... Args>
	bool mysql_assign_next_row(Args... args){
		// WARNING: Shares row.
		return compsky::mysql::assign_next_row(this->res[i], &this->row, args...);
	}
	
	template<size_t i = 0,  typename... Args>
	bool mysql_assign_next_row__no_free(Args... args){
		return compsky::mysql::assign_next_row__no_free(this->res[i], &this->row, args...);
	}
	
	template<size_t i = 0>
	void mysql_free_res(){
		mysql_free_result(this->res[i]);
	}
	
	template<size_t i = 0>
	void mysql_seek(const int j){
		mysql_data_seek(this->res[i], j);
	}
	
	std::string_view get_buf_as_string_view(){
		return std::string_view(this->buf, this->buf_indx());
	}
	
#ifdef n_cached
	void add_buf_to_cache(const unsigned int which_cached_fn,  const uint64_t user_id,  const unsigned int n_requests = 1){
		cached_stuff::add(this->buf, this->buf_indx(), which_cached_fn, user_id, n_requests);
	}
#endif
	
	
	template<size_t i = 0,  typename... Args>
	char* get_itr_from_buf(std::nullptr_t,  const char* const _headers,  Args... args){
		size_t sz = 0; // NOTE: If there is a free(): corrupted chunk error, it is probably because strlens has calculated the wrong size for the container.
		
		sz += std::char_traits<char>::length(_headers);
		sz += 1;
		sz += _r::get_size_of_json_response_rows_from_sql_res(this->res[i], &this->row, args...);
		sz += 1;
		sz += 1;
		
		void* buf = malloc(sz);
		if(unlikely(buf == nullptr))
			exit(4096);
		return reinterpret_cast<char*>(buf);
	}
	template<typename... Args>
	char* get_itr_from_buf(char** buf,  const char* const,  Args...){
		return *buf;
	}
	void set_buf_to_itr(std::nullptr_t, char*){}
	void set_buf_to_itr(char** buf,  char* itr){
		*buf = itr;
	}
	void set_buf_to_itr(const char** buf,  char* itr){
		*buf = const_cast<const char*>(itr);
	}
	
	template<size_t i = 0,  typename ArrOrDict,  typename... Args>
	bool asciify_json_response_rows(char*& itr,  const ArrOrDict f_arr_or_dict,  Args... args){
		_r::asciify_json_response_rows_from_sql_res(this->res[i], &this->row, itr, f_arr_or_dict, args...);
		const bool rc = (likely(*(itr - 1) == ','));
		if (rc)
			--itr;
		return rc;
	}
	
	template<size_t i = 0,  typename ArrOrDict,  typename... Args>
	bool init_json_rows(char*& itr,  const ArrOrDict _flag,  Args... args){
		compsky::asciify::asciify(itr, _r::opener_symbol(_flag));
		const bool rc = this->asciify_json_response_rows<i>(itr, _flag, args...);
		*(itr++) = _r::closer_symbol(_flag);
		return rc;
	}
	
	template<size_t i = 0,  typename StackdBuf,  typename MallocdBuf,  typename ArrOrDict,  typename... Args>
	void init_json(const StackdBuf stacked_itr,  const ArrOrDict _flag,  MallocdBuf mallocd_dst,  Args... args){
		/*
		 * stacked_itr is either nullptr or this->itr
		 * In the first case,  itr_init is a new malloc'd string that is assigned to mallocd_dst
		 * In the latter case, this->itr is incremented so that a string_view of this->buf can be replied
		 */
		
		char* const itr_init = this->get_itr_from_buf(stacked_itr, _r::json_init, args...);
		char* itr = itr_init;
		
		compsky::asciify::asciify(itr, _r::json_init);
		this->mysql_seek(0); // Reset to first result
		this->init_json_rows<i>(itr, _flag, args...);
		*itr = 0;
		
		this->set_buf_to_itr(mallocd_dst, itr_init);
		this->set_buf_to_itr(stacked_itr, itr);
	}
	
	void begin_json_response(char*& itr){
		compsky::asciify::asciify(itr, _r::json_init);
	}
	
	void begin_json_response(){
		this->reset_buf_index();
		this->begin_json_response(this->itr);
	}
	
	template<size_t i = 0,  typename... Args>
	void write_json_list_response_into_buf(Args... args){
		this->begin_json_response();
		this->init_json_rows<i>(this->itr, _r::flag::arr, args...);
		*this->itr = 0;
	}
};

}
}
