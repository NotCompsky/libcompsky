#include "compsky/asciify/base.hpp"


namespace compsky {
namespace asciify {

#ifdef COMPILING_STATIC_LIB
extern char* BUF;
#else
char* BUF;
#endif

}
}
