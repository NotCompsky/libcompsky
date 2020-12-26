#pragma once

#include "stdinout_defines.hpp"


namespace compsky {
namespace os {


inline
bool del_file(const char* const path){
#ifdef _WIN32
#else
	return remove(path);
#endif
}


}
}
