#pragma once


#ifdef _WIN32
# include <cstdio>
# define STDIN_FILE_ID  stdin
# define STDOUT_FILE_ID stdout
# define STDERR_FILE_ID stderr
#else
# include <unistd.h>
# define STDIN_FILE_ID  STDIN_FILENO
# define STDOUT_FILE_ID STDOUT_FILENO
# define STDERR_FILE_ID STDERR_FILENO
#endif


namespace compsky {
namespace os {


typedef typeof(STDOUT_FILE_ID) fileid_typ;


}
}
