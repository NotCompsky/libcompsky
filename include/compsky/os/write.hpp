#pragma once

#include "stdinout_defines.hpp"
#include "open.hpp"
#ifndef _WIN32
# include <unistd.h>
#endif
#include <compsky/asciify/asciify.hpp>
#include <compsky/utils/ptrdiff.hpp>


namespace compsky {
namespace os {


inline
bool write_n_bytes(const fileid_typ file_id,  const char* io_buf,  const std::size_t n_bytes){
  #ifdef _WIN32
	return (unlikely(fwrite(io_buf, n_bytes, 1, file_id) != 1));
  #else
	return (unlikely(write(file_id, io_buf, n_bytes) != n_bytes));
  #endif
}

inline
bool write_n_bytes_at_offset(const fileid_typ file_id,  const char* buf,  const std::size_t n_bytes,  const std::size_t offset){
  #ifdef _WIN32
	#error "write_n_bytes_at_offset for Windows is Not implemented yet"
  #else
	return (unlikely(pwrite(file_id, buf, n_bytes, offset) != n_bytes));
  #endif
}

template<typename... Args>
bool write_to_file(const fileid_typ file_id,  char* buf,  Args&&... args){
	char* itr = buf;
	compsky::asciify::asciify(itr, args...);
	return write_n_bytes(file_id, buf,  utils::ptrdiff(itr, buf));
}

template<typename... Args>
bool write_to_stdout(Args&&... args){
	return write_to_file(STDOUT_FILE_ID, args...);
}

template<typename... Args>
bool write_to_stderr(Args&&... args){
	return write_to_file(STDERR_FILE_ID, args...);
}


class WriteOnlyFile {
 private:
	const fileid_typ f_id;
 public:
	WriteOnlyFile(const char* const fp)
	: f_id(open_file_for_writing(fp))
	{}
	
	~WriteOnlyFile(){
		if (not this->is_null())
			compsky::os::close_file_handle(this->f_id);
	}
	
	bool is_null() const {
		return (this->f_id == 0);
	}
	
	template<typename... Args>
	bool write(Args&&... args){
		return write_to_file(this->f_id, args...);
	}
	
	bool write_from_buffer(const char* buf,  const std::size_t n_bytes){
		return write_n_bytes(this->f_id, buf, n_bytes);
	}
	
	bool write_from_buffer_at_offset(const char* buf,  const std::size_t n_bytes,  const std::size_t offset){
		return write_n_bytes_at_offset(this->f_id, buf, n_bytes, offset);
	}
};


}
}
