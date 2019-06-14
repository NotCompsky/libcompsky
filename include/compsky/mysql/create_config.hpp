#ifndef __COMPSKY__MYSQL__CREATECONFIG__
#define __COMPSKY__MYSQL__CREATECONFIG__

#include "compsky/mysql/mysql.h"


namespace compsky {
namespace mysql {

extern MYSQL_RES* RES1;
extern MYSQL_ROW ROW1;

extern char* AUTH_PTR;

void ef_reed();

void create_config(const char* stmts,  const char* env_var = 0);

}
} // END namespace compsky::mysql

#endif
