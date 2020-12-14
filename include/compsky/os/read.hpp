#pragma once

#include "stdinout_defines.hpp"
#include "open.hpp"
#ifndef _WIN32
# include <unistd.h>
#endif


namespace compsky {
namespace os {


inline
bool read_from_file(const fileid_typ file_id,  char* buf,  const size_t n_bytes){
  #ifdef _WIN32
	return (unlikely(fread(buf, n_bytes, 1, file_id) != 1));
  #else
	return (read(file_id, buf, n_bytes) != n_bytes);
  #endif
}


template<typename... Args>
bool read_from_stdin(Args&&... args){
	return read_from_file(STDIN_FILE_ID, args...);
}


class ReadOnlyFile {
 private:
	const fileid_typ f_id;
 public:
	ReadOnlyFile(const char* const fp)
	: f_id(open_file_for_reading(fp))
	{}
	
	~ReadOnlyFile(){
		if (not this->is_null())
			compsky::os::close_file_handle(this->f_id);
	}
	
	bool is_null() const {
		return (this->f_id == 0);
	}
	
	template<typename... Args>
	bool read_into_buf(Args&&... args){
		return read_from_file(this->f_id, args...);
	}
};


}
}
