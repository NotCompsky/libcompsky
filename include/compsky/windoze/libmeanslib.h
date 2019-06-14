// Trick Windows into building the lib files that it requires.
// Already achieved with -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE - the issue was that the resulting lib files are not exported and need to be moved out of the CMake cache by hand.

#ifndef __COMPSKY_LIBMEANSLIB__

#ifdef _WIN32
  #define __declspec(dllexport)
#else
  #define HELLO
#endif

#endif
