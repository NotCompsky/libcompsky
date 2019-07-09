#ifndef LIBCOMPSKY_ASCIIFY_PRINT_HPP
#define LIBCOMPSKY_ASCIIFY_PRINT_HPP


#include <compsky/asciify/asciify.hpp>

#include <stdio.h> // for FILE


namespace compsky {
namespace asciify {

template<typename... Args>
void write(FILE* f,  Args... args){
    BUF_INDX = 0;
    asciify(args...);
    fwrite(BUF,  1,  BUF_INDX,  f);
};

} // END namespace compsky::asciify
} // END namespace compsky


#endif
