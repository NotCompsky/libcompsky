#ifndef LIBCOMPSKY_MYSQL_CREATE_CONFIG_HPP
#define LIBCOMPSKY_MYSQL_CREATE_CONFIG_HPP

#include "compsky/mysql/mysql.h"


namespace compsky {
namespace mysql {

extern MYSQL_RES* RES1;
extern MYSQL_ROW ROW1;

extern char* AUTH_PTR;

void ef_reed();

void create_config(const char* stmts,  const char* permissions_str,  const char* env_var = 0);

}
} // END namespace compsky::mysql

#endif
