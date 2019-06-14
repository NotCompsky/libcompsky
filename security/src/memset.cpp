#include "compsky/security/memset.hpp"



#include <string.h> // for memset

#ifdef _WIN32
  #include <windows.h>
#endif


namespace compsky {
namespace security {

void memzero_secure(void* data,  size_t len){
    // Based on work by Zhaomo Yang, of the University of California, who released it into the public domain.
  #ifdef _WIN32
    SecureZeroMemory (data, len);
  #elif defined(__GNUC__) || defined(__clang__)
    memset(data, 0, len);
    __asm__ __volatile__("" : : "r"(data) : "memory");
  #else
    volatile char *p = (volatile char *) data;
    while (len--)
      *p++ = 0;
  #endif
}

} // END: namespace compsky::security
}
