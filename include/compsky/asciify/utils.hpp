#ifndef LIBCOMPSKY_ASCIIFY_UTILS_HPP
#define LIBCOMPSKY_ASCIIFY_UTILS_HPP


#include <stddef.h> // for size_t


template<unsigned base = 10,  typename T>
size_t count_digits(T n){
    size_t n_digits = 0;
    do {
        ++n_digits;
		n /= base;
    } while (n != 0); // Note that "0" has 1 digit, so a while{} loop cannot be used.
    return n_digits;
};


#endif
