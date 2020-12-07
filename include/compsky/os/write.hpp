#pragma once

#include "stdinout_defines.hpp"
#ifndef _WIN32
# include <unistd.h>
#endif


namespace compsky {
namespace os {


inline
bool write_n_bytes(const fileid_typ file_id,  const char* io_buf,  const size_t n_bytes){
  #ifdef _WIN32
	return (unlikely(fwrite(io_buf, n_bytes, 1, file_id) != 1));
  #else
	return (write(file_id, io_buf, n_bytes) != n_bytes);
  #endif
}

template<size_t buf_sz,  typename... Args>
bool write_to_file(const fileid_typ file_id,  char (&buf)[buf_sz],  Args&&... args){
	char* itr = buf;
	compsky::asciify::asciify(itr, args...);
	write_n_bytes(file_id, buf,  (uintptr_t)itr - (uintptr_t)buf);
}

template<typename... Args>
bool write_to_stdout(Args&&... args){
	return write_to_file(STDOUT_FILE_ID, args...);
}

template<typename... Args>
bool write_to_stderr(Args&&... args){
	return write_to_file(STDERR_FILE_ID, args...);
}


}
}
