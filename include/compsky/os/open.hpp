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
void close_file_handle(const fileid_typ fd){
  #ifdef _WIN32
	fclose(fd);
  #else
	close(fd);
  #endif
}


}
}
