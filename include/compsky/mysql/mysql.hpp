#ifndef LIBCOMPSKY_MYSQL_MYSQL_HPP
#define LIBCOMPSKY_MYSQL_MYSQL_HPP

#include "compsky/mysql/mysql.h" // for mysql.h


namespace compsky {
namespace mysql {

namespace flag {
    struct SizeOfAssigned{};
}

#define AUTH_SZ 512 // Sane max size for MySQL authorisation/config file

#ifndef _WIN32
extern char* AUTH;
#else
extern char AUTH[AUTH_SZ];
#endif
extern char* MYSQL_AUTH[6];

extern MYSQL* OBJ;


void init_auth(const char* fp);
void login_from_auth();
void init(const char* fp);

void exit_mysql();






extern MYSQL_RES* RES; // Not used in this header, only for convenience
extern MYSQL_ROW ROW; // Not used in this header, only for convenience


void exec_buffer(const char* s,  const size_t sz);
void exec_buffer(const char* s);

void query_buffer(MYSQL_RES** res,  const char* s,  const size_t sz);
void query_buffer(MYSQL_RES** res,  const char* s);

void assign_next_column(MYSQL_ROW row,  int* col);

void free_result();

}
} // END namespace compsky::mysql

#endif
