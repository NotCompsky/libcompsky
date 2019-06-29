#include "compsky/asciify/base.hpp"

#include <string.h> // for memcpy

#include "compsky/asciify/core.hpp"


namespace compsky {
namespace asciify {

#ifdef COMPILING_STATIC_LIB
extern char* BUF;
#else
char* BUF;
#endif

/* Base Cases */
void asciify(){}

void asciify(const char* const s){
    for (auto i = 0;  s[i] != 0;  ++i){
        BUF[BUF_INDX++] = s[i];
    }
}

void asciify(const char c){
    BUF[BUF_INDX++] = c;
}

void asciify(flag::StrLen f,  const char* const s,  const int sz){
    memcpy(BUF + BUF_INDX,  s,  sz);
    BUF_INDX += sz;
};

void asciify(flag::StrLen f,  const char* const s,  const size_t sz){
    memcpy(BUF + BUF_INDX,  s,  sz);
    BUF_INDX += sz;
};

void asciify(flag::prefix::Start f,  const char* const s,  const size_t sz,  const char* const ss){
    constexpr static const flag::StrLen g;
    asciify(g, s, sz);
    asciify(ss);
};

}
}
