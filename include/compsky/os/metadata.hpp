#pragma once

#include <compsky/macros/likely.hpp>
#ifdef _WIN32
#else
# include <sys/stat.h>
#endif


namespace compsky {
namespace os {


inline
std::size_t get_file_sz(const char* const fp){
  #ifdef _WIN32
	HANDLE const f = CreateFileA(fp,  GENERIC_READ,  0,  nullptr,  OPEN_EXISTING,  FILE_ATTRIBUTE_NORMAL,  nullptr);
	if (unlikely(f == INVALID_HANDLE_VALUE))
		handler(CANNOT_CREATE_FILE);
	_LARGE_INTEGER f_sz; // For x86_32 compatibility
	if (unlikely(GetFileSizeEx(f, &f_sz) == 0))
		handler(COULD_NOT_GET_FILE_SIZE);
	CloseHandle(f);
	return f_sz.QuadPart;
  #else
	static struct stat stat_buf;
	const auto rc3 = stat(fp, &stat_buf);
	if (unlikely(rc3 == -1))
		return -1; // aka std::numeric_limits<uint64_t>::max() ?
	return stat_buf.st_size;
  #endif
}


}
}
