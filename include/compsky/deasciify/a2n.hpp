#pragma once

#include "char_value.hpp"
#include "increment.hpp"


template<typename Int,  typename StrOrStrPtr,  bool can_be_negative = false>
constexpr
Int a2n(StrOrStrPtr s){
	Int n = 0;
	if constexpr (can_be_negative){
		if (char_value(s) == '-'){
			increment(&s);
			return -a2n<Int, StrOrStrPtr, can_be_negative>(s);
		}
	}
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
