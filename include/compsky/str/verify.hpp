#pragma once


namespace compsky {
namespace str {



constexpr
bool is_integral_char(const char c){
	return ((c >= '0')  and  (c <= '9'));
}



template<char separator = ',',  char terminator = '\0'>
const char* get_int_csv(const char*& str){
	const char* const start = str;
	while(true){
		if (not is_integral_char(*str))
			return nullptr;
		do {
			++str;
		} while (is_integral_char(*str));
		
		if (*str == separator){
			++str;
			continue;
		}
		
		return (*str == terminator) ? start : nullptr;
	}
}



}
}
