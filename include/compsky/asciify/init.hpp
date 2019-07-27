#ifndef LIBCOMPSKY_ASCIIFY_INIT_HPP
#define LIBCOMPSKY_ASCIIFY_INIT_HPP

#include "compsky/asciify/init_h.hpp"

#include <cstdlib> // for malloc


#ifndef likely
    #ifdef __GNUC__
        #define likely(x)       __builtin_expect(!!(x), 1)
        #define unlikely(x)     __builtin_expect(!!(x), 0)
    #else
        #define likely(x)       (x)
        #define unlikely(x)     (x)
    #endif
#endif


namespace compsky {
namespace asciify {


extern char* BUF;
extern char* ITR;


inline
bool alloc(const size_t sz){
    ITR = BUF = (char*)malloc(sz);
    return (unlikely(ITR == nullptr));
    // NOTE: The testing of ITR rather than BUF is because ITR is most likely to be used afterwards.
}


} // end namespace asciify
} // end namespace compsky


#endif
