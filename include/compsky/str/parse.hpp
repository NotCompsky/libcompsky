#pragma once

#include <cstdint>


namespace compsky {
namespace str {


constexpr
std::size_t count_until(const char* s,  const char c){
	// NOTE: Not inclusive
	std::size_t n = 0;
	while((*s != c) and (*s != 0)){
		++s;
		++n;
	}
	return n;
}


constexpr
void replace_first_instance_of(char* str,  const char a,  const char b){
	// str is guaranteed to be max characters long
	while(*str != 0){
		if(*str == a){
			*str = b;
			return;
		}
		++str;
	}
}

template<std::size_t N>
void replace_first_instance_of(char(&str)[N],  const char a,  const char b){
	// str is guaranteed to be max characters long
	for(std::size_t i = 0;  i < N;  ++i){
		if(str[i] == a){
			str[i] = b;
			return;
		}
	}
}

constexpr
void replace_first_instance_of(char* str,  const char a,  const char* b,  const char c){
	// str is guaranteed to be max characters long
	while(*str != 0){
		if(*str == a){
			while(*b != 0){
				*(str++) = *(b++);
			}
			// WARNING: No bounds checking (laziness)
			*str = c;
			return;
		}
	}
}


} // namespace str
} // namespace compsky
