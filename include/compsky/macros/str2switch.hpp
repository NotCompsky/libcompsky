#pragma once


#include <boost/preprocessor/repetition/repeat.hpp>
#include <cstddef> // for size_t


template<size_t N>
constexpr char char_at(char const(&s)[N],  const size_t i){
	return s[i];
}

#define SEPARATOR1_1(n) switch(*(++str)){ case
#define SEPARATOR1_2() :
#define SEPARATOR2_2() break;
#define SEPARATOR2_1() }
#define STR2SWITCH1(z, n, data) \
	SEPARATOR1_1(n) \
	char_at(data, n) \
	SEPARATOR1_2()
#define STR2SWITCH2(z, n, data) \
	SEPARATOR2_2() \
	SEPARATOR2_1()

#define STR2SWITCH(STRLEN, STR, RETURN)  \
	BOOST_PP_REPEAT(STRLEN, STR2SWITCH1, STR) \
	RETURN \
	BOOST_PP_REPEAT(STRLEN, STR2SWITCH2, STR)
