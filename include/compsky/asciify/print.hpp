#ifndef LIBCOMPSKY_ASCIIFY_PRINT_HPP
#define LIBCOMPSKY_ASCIIFY_PRINT_HPP


#include <compsky/asciify/asciify.hpp>

#include <stdio.h> // for FILE


namespace compsky {
namespace asciify {

template<typename... Args>
void write(FILE* f,  Args... args){
    asciify(args...);
    fwrite(BUF,  1,  (uintptr_t)ITR - (uintptr_t)BUF,  f);
};

} // END namespace compsky::asciify
} // END namespace compsky


#endif
