#pragma once


#include <boost/preprocessor/repetition/repeat.hpp>
#include <cstddef> // for size_t


template<size_t N>
constexpr char char_at(char const(&s)[N],  const size_t i){
	return s[i];
}

#define STUFF(z, n, data) or (c == char_at(data, n))

#define CHAR_IN_ARR(CHAR, STRLEN, STR)  \
	[](const char c)->bool { \
		return (false BOOST_PP_REPEAT(STRLEN, STUFF, STR)); \
	}(CHAR)
