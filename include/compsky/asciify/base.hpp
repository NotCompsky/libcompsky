#ifndef __ASCIIFY_BASE__
#define __ASCIIFY_BASE__



#ifdef QT_GUI_LIB
  #include <QString>
#endif
#include <cstddef> // for size_t
#include "compsky/asciify/flags.hpp"


namespace compsky {
namespace asciify {

extern char* BUF;

/* Base Cases */
void asciify();

void asciify(const char* const s);

void asciify(const char c);

void asciify(flag::StrLen f,  const char* const s,  const int sz);

void asciify(flag::StrLen f,  const char* const s,  const size_t sz);

void asciify(flag::prefix::Start f,  const char* const s,  const size_t sz,  const char* const ss);

}
}

#endif
