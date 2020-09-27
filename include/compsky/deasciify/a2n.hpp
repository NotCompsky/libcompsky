#pragma once

#include "char_value.hpp"
#include "increment.hpp"


template<typename Int,  typename StrOrStrPtr>
constexpr
Int a2n(StrOrStrPtr s){
	Int n = 0;
#ifdef LIBCOMPSKY_NO_TESTS
	while(char_value(s) != 0)
#else
	while(char_value(s) >= '0'  &&  char_value(s) <= '9')
#endif
	{
		n *= 10;
		n += char_value(s) - '0';
		increment(&s);
	}
	return n;
}
