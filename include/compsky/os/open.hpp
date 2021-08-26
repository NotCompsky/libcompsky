#pragma once

#include "stdinout_defines.hpp"
#ifndef _WIN32
# include <fcntl.h> // for open, O_RDONLY, O_WRONLY
#endif


namespace compsky {
namespace os {


inline
fileid_typ open_file_for_reading(const char* const file_path){
  #ifdef _WIN32
	return fopen(file_path, "rb");
  #else
	return open(file_path, O_RDONLY);
  #endif
}


inline
fileid_typ open_file_for_writing(const char* const file_path){
  #ifdef _WIN32
	return fopen(file_path, "wb");
  #else
	return open(file_path, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU|S_IRWXG);
  #endif
}


inline
void close_file_handle(const fileid_typ fd){
  #ifdef _WIN32
	fclose(fd);
  #else
	close(fd);
  #endif
}


}
}
