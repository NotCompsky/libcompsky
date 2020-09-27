#pragma once

namespace compsky {
namespace utils {
inline
bool streq(const char* a,  const char* b){
	while(*a != 0){
		if (*a != *b)
			return false;
		++a;
		++b;
	}
	return (*b == 0);
}
}
}
