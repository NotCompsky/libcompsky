#include "compsky/asciify/base.hpp"


namespace compsky {
namespace asciify {

#ifdef COMPILING_STATIC_LIB
extern char* BUF;
extern char* ITR;
#else
char* BUF;
char* ITR;
#endif

}
}
