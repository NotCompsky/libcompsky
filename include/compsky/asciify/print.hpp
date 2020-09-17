#ifndef LIBCOMPSKY_ASCIIFY_PRINT_HPP
#define LIBCOMPSKY_ASCIIFY_PRINT_HPP


#include <compsky/asciify/asciify.hpp>

#include <stdio.h> // for FILE


namespace compsky {
namespace asciify {

template<typename... Args>
void write(char* const buf,  FILE* f,  Args... args){
	char* itr = buf;
	asciify(itr, args...);
    fwrite(buf,  1,  (uintptr_t)itr - (uintptr_t)buf,  f);
};

} // END namespace compsky::asciify
} // END namespace compsky


#endif
