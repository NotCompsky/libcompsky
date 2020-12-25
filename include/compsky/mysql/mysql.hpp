#ifndef LIBCOMPSKY_MYSQL_MYSQL_HPP
#define LIBCOMPSKY_MYSQL_MYSQL_HPP

#include "compsky/mysql/mysql.h" // for mysql.h
#include "compsky/mysql/except.hpp" // for compsky::mysql::except
#include "compsky/os/read.hpp"

#include <string.h> // for strlen
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


struct MySQLAuth {
	union {
		char* bufs[6];
		struct {
			char* host;
			char* path;
			char* user;
			char* pwrd;
			char* dbnm;
			char* port;
		};
	};
	const char* operator[](int i) const {
		// Implements the override for ? = MySQLAuth[i]
		return this->bufs[i];
	}
	char*& operator[](int i){
		// Implements the override for MySQLAuth[i] = ?
		return this->bufs[i];
	}
};


template<size_t buf_sz>
void init_auth(char (&buf)[buf_sz],  MySQLAuth& mysql_auth,  const char* const fp){
	// Reads contents of 'fp' into 'buf', and then parses it into the string array 'mysql_auth'
	if (unlikely(fp == nullptr))
		throw except::Nullptr();
	
	compsky::os::ReadOnlyFile f(fp);
	
	if (unlikely(f.is_null()))
		throw except::FileOpen(fp);
	
	f.read_into_buf(buf, buf_sz);
	
    auto n_lines = 0;
    mysql_auth[0] = buf + 6;
    for (char* itr = mysql_auth[0];  n_lines < 5;  ++itr){
        if (*itr == '\n'){
            *itr = 0;
            itr += 7; // To skip "\nABCD: "
            mysql_auth[++n_lines] = itr;
        }
	}
}

inline
void login_from_auth(MYSQL*& mysql_obj,  const MySQLAuth mysql_auth){
	const char* path = mysql_auth.path;
	const char* pwrd = mysql_auth.pwrd;
    if ((path[0] == '/') and (path[1] == 0))
        path = NULL;
    if (pwrd[0] == 0)
        pwrd = NULL;
    
    unsigned int port_n = 0;
	char* itr = mysql_auth.port;
    while (*itr >= '0'  &&  *itr <= '9'){
        port_n *= 10;
        port_n += *itr - '0'; // Integers are continuous in every realistic character encoding
        ++itr;
    }
    
    mysql_obj = mysql_init(NULL);
	
    if (unlikely(!mysql_obj))
		throw except::OOM();
    
    if (unlikely(!mysql_real_connect(mysql_obj, mysql_auth.host, mysql_auth.user, pwrd, mysql_auth.dbnm, port_n, path, 0)))
		throw except::ConnectToServer(mysql_auth.user, pwrd, mysql_auth.host, port_n, path);
}

template<size_t buf_sz>
void init(MYSQL*& mysql_obj,  char (&buf)[buf_sz],  const char* const fp){
	MySQLAuth mysql_auth;
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
