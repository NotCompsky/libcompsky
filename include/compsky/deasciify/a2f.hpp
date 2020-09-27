#pragma once

#include "char_value.hpp"
#include "increment.hpp"


template<typename Float,  typename StrOrStrPtr>
constexpr
Float a2f(StrOrStrPtr s){
	uint64_t ns[2] = {0, 0};
	unsigned indx = 0;
	uint64_t divisor = 1;
	while(true){
		if(char_value(s) == '.'){
			if(indx)
				// If we have already encountered a decimal point
				return 0.0;
			++indx;
			divisor = 1; // Ignore the divisor_power set from the pre-decimal part
			increment(&s);
			continue;
		}
		if((char_value(s) < '0') || (char_value(s) > '9'))
			return (Float)ns[0] + (Float)ns[1] / (Float)divisor;
		ns[indx] *= 10;
		ns[indx] += char_value(s) - '0';
		divisor *= 10;
		increment(&s);
	}
}
