#ifndef LIBCOMPSKY_MYSQL_MYSQL_HPP
#define LIBCOMPSKY_MYSQL_MYSQL_HPP

#include "compsky/mysql/mysql.h" // for mysql.h
#include "compsky/mysql/except.hpp" // for compsky::mysql::except

#include <string.h> // for strlen
#include <stdio.h> // for fopen, fread
#ifndef _WIN32
# include <sys/mman.h> // for mmap, munmap
#endif
#include <compsky/security/memset.hpp> // for compsky::security::memzero_secure
#include "compsky/macros/likely.hpp"


/*
Format of config file must be exactly:

HOST: some.domain.name.or.localhost
PATH: /path/to/unix/socket/or/blank/if/not/using/it
USER: your_username
PWD:  your_password
DB:   your_database_name
PORT: 12345 (or 0 if not using port)
SOME_EXTRA_LINE_HERE_THAT_ISNT_PARSED_BUT_JUST_TO_KEEP_NEWLINE
*/


namespace compsky {
namespace mysql {

namespace flag {
    struct SizeOfAssigned{};
}


template<size_t buf_sz>
void init_auth(char (&buf)[buf_sz],  char* mysql_auth[6],  const char* const fp){
	// Reads contents of 'fp' into 'buf', and then parses it into the string array 'mysql_auth'
	if (unlikely(fp == nullptr))
		throw except::Nullptr();
    FILE* f = fopen(fp, "rb");
	
	if (unlikely(f == 0))
		throw except::FileOpen(fp);
	
    if (unlikely(fread(buf, 1, buf_sz, f) == 0)){
		fclose(f);
		throw except::FileRead(fp);
	}
	
    auto n_lines = 0;
    mysql_auth[0] = buf + 6;
    for (char* itr = mysql_auth[0];  n_lines < 5;  ++itr){
        if (*itr == '\n'){
            *itr = 0;
            itr += 7; // To skip "\nABCD: "
            mysql_auth[++n_lines] = itr;
        }
	}
    fclose(f);
}

inline
void login_from_auth(MYSQL*& mysql_obj,  char* mysql_auth[6]){
    const char* host = mysql_auth[0];
    const char* path = mysql_auth[1];
    const char* user = mysql_auth[2];
    const char* pwrd = mysql_auth[3];
    const char* dbnm = mysql_auth[4];
    
    if ((path[0] == '/') and (path[1] == 0))
        path = NULL;
    if (pwrd[0] == 0)
        pwrd = NULL;
    
    unsigned int port_n = 0;
    char* itr = mysql_auth[5];
    while (*itr >= '0'  &&  *itr <= '9'){
        port_n *= 10;
        port_n += *itr - '0'; // Integers are continuous in every realistic character encoding
        ++itr;
    }
	
	printf("STUFF\nhost %s\nuser %s\npwrd %s\ndbnm %s\nport %u\npath %s\n", host, user, pwrd, dbnm, port_n, path);
	fflush(stdout);
    
    mysql_obj = mysql_init(NULL);
	
    if (unlikely(!mysql_obj))
		throw except::OOM();
    
    if (unlikely(!mysql_real_connect(mysql_obj, host, user, pwrd, dbnm, port_n, path, 0)))
		throw except::ConnectToServer(user, pwrd, host, port_n, path);
}

template<size_t buf_sz>
void init(MYSQL*& mysql_obj,  char (&buf)[buf_sz],  const char* const fp){
	char* mysql_auth[6];
	init_auth(buf, mysql_auth, fp);
    login_from_auth(mysql_obj, mysql_auth);
}

inline
void wipe_auth(char* const buf,  const size_t buf_sz){
    compsky::security::memzero_secure(buf, buf_sz);
# ifndef _WIN32
    munmap(buf, buf_sz);
# endif
}




inline
void exec_buffer(MYSQL* const obj,  const char* const s,  const size_t sz){
#ifdef DEBUG
	printf("EXEC: %.*s\n", (int)sz, s);
#endif
    if (likely(mysql_real_query(obj, s, sz) == 0))
        return;
	throw except::SQLExec(s, sz);
};

inline
void exec_buffer(MYSQL* const obj,  const char* s){
    exec_buffer(obj, s, strlen(s));
};

inline
void query_buffer(MYSQL* const obj,  MYSQL_RES*& res,  const char* const s,  const size_t sz){
#ifdef DEBUG
	printf("QRY: %.*s\n", (int)sz, s);
#endif
    if (likely(mysql_real_query(obj, s, sz) == 0)){
        res = mysql_store_result(obj);
        return;
    }
    throw except::SQLExec(s, sz);
};

inline
void query_buffer(MYSQL* const obj,  MYSQL_RES*& res,  const char* const s){
    query_buffer(obj, res, s, strlen(s));
};


}
} // END namespace compsky::mysql

#endif
