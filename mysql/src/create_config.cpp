#include "compsky/mysql/create_config.hpp"



#include <stdio.h> // for fprintf
#include <iostream> // for std::cout
#include <string> // for std::char_traits

#include <compsky/asciify/flags.hpp>
#include <compsky/mysql/query.hpp>

#ifdef _WIN32
  #include <windows.h>
#endif


#define PROMPT_READ_INTO_AUTH_PTR(...) \
	std::cout << __VA_ARGS__ << std::flush; \
	_details::ef_reed(auth_ptr);
#define CREATE_CFG_LINE(prompt, line_prefix) \
	memcpy(auth_ptr, line_prefix, std::char_traits<char>::length(line_prefix)); \
	auth_ptr += std::char_traits<char>::length(line_prefix); \
	mysql_auth[i] = auth_ptr; \
	PROMPT_READ_INTO_AUTH_PTR(prompt) \
	auth_ptr_ENDS[i] = auth_ptr; \
	++i


namespace _details {
static
void ef_reed(char*& auth_ptr){
    char c;
    while((c = fgetc(stdin))){
        if (c != '\n'){
            *auth_ptr = c;
            ++auth_ptr;
        } else return;
    }
}
}

namespace _f {
	constexpr static const compsky::asciify::flag::Escape esc;
}

namespace indx {
	enum {
		host,
		path,
		user,
		pwrd,
		dbnm,
		port,
		COUNT
	};
}


namespace compsky {
namespace mysql {



void create_config(const char* const stmts,  const char* const permissions_str,  const char* const env_var){
	MYSQL* mysql_obj;
	char* mysql_auth[indx::COUNT];
	
    std::cout << "* MySQL Configuration *" << std::endl;
    
    std::cout << "Absolute file path to save the config file to (will NOT create folders/directories for you): " << std::flush;
    
    char buf[4096];
	char* buf_itr = buf;
	char* const cfg_pth = buf_itr;
	_details::ef_reed(buf_itr);
	*(buf_itr++) = 0;
    
    char* auth = buf_itr;
    char* auth_ptr_ENDS[indx::COUNT];
    char* auth_ptr = auth;
    auto i = 0;
    
	CREATE_CFG_LINE(
	  #ifdef _WIN32
		"Host (127.0.0.1 if it is on this machine): " // localhost might refer to IPv6 ::1:
	  #else
		"Host (localhost if it is on this machine): " // 127.0.0.1 causes issue with Unix socket // TODO: Check if this is also the issue with Windows named socket
	  #endif
		, "HOST: "
	);
    
    bool is_localhost = (strncmp(mysql_auth[indx::host], "localhost", strlen("localhost")) == 0)  ||  (strncmp(mysql_auth[indx::host], "127.0.0.1", strlen("127.0.0.1")) == 0);
    
	CREATE_CFG_LINE(
		"Socket file path or named pipe name (a single '/' character if not used)"
	  #ifdef _WIN32
		": "
	  #else
		"\nHint: you can run `mysql_config --socket` to find it, but it is probably `/var/run/mysqld/mysqld.sock`\nYou can set the location in the MySQL config file, which is probably `/etc/mysql/my.cnf` - restart the MySQL server service if you do.\nSocket file path: "
	  #endif
		, "\nPATH: "
	);
    
    std::cout << "A user will be now created for the program to use, that will only have access to the one database it uses:" << std::endl;
    
	CREATE_CFG_LINE("  Username: ", "\nUSER: ");
	CREATE_CFG_LINE("  Password: ", "\nPWRD: ");
	CREATE_CFG_LINE("Database name: ", "\nDBNM: ");
	CREATE_CFG_LINE("MySQL Server port number (blank if not connecting via TCP/IP): ", "\nPORT: ");
    
    *auth_ptr = '\n'; // Terminate port number calculation
	std::cout << std::endl;
    
    FILE* cfg = fopen(cfg_pth, "wb");
    fwrite(auth,  1,  (uintptr_t)auth_ptr + 1 - (uintptr_t)auth,  cfg);
    fclose(cfg);
    
    for (auto j = 0;  j < indx::COUNT;  ++j)
        auth_ptr_ENDS[j][0] = 0;
    for (auto j = 0;  j < indx::COUNT;  ++j)
        printf("mysql_auth[%d] = %s\n", j, mysql_auth[j]);
    
    /* Now to login to the MySQL database with the root user */
    
    const char* username = mysql_auth[indx::user]; // User to grant permissions to
    const char* password = mysql_auth[indx::pwrd]; // His password
    
    mysql_auth[indx::user] = auth_ptr;
	PROMPT_READ_INTO_AUTH_PTR("MySQL admin username: ")
    *auth_ptr = 0; // So we can write it out here:
    ++auth_ptr;
    
    mysql_auth[indx::pwrd] = auth_ptr;
	PROMPT_READ_INTO_AUTH_PTR("MySQL admin password (leave blank to use system socket authentication - i.e. if you can login to MySQL as `" << mysql_auth[indx::user] << "` without a password): ")
    *auth_ptr = 0;
    
    const char* db_name = mysql_auth[indx::dbnm];
    
	mysql_auth[indx::dbnm] = nullptr; // Connect to 'null' database, since we haven't created our database yet
    
    login_from_auth(mysql_obj, mysql_auth);
    
    char* const mysql_exec_buffer = auth_ptr + 1; // Buffer for the following MySQL commands
    
    exec(mysql_obj, mysql_exec_buffer, "CREATE DATABASE IF NOT EXISTS `", db_name, "`");
    
    mysql_select_db(mysql_obj, db_name); // Set as current database
	
	if (stmts != nullptr){
		const char* last_stmt = stmts;
		for (const char* itr = stmts;  *itr != 0;  ++itr){
			if (unlikely(*itr == ';')){
				exec_buffer(mysql_obj,  last_stmt,  (uintptr_t)itr - (uintptr_t)last_stmt);
				last_stmt = itr + 1;
			}
		}
	}
    
    if (is_localhost){
		exec(mysql_obj, mysql_exec_buffer, "CREATE USER IF NOT EXISTS `", _f::esc, '`', username, "`@`localhost` IDENTIFIED BY \"", _f::esc, '"', password, "\"");
		exec(mysql_obj, mysql_exec_buffer, "GRANT ", permissions_str, " ON ", db_name, ".* TO `", _f::esc, '`', username, "`@`localhost`");
    } else {
        std::cout << "You must manually create the user `" << username << "` and grant him permissions: SELECT, INSERT, UPDATE on `" << db_name << "`" << std::endl;
    }
    
    if (env_var == nullptr)
        return;
    
#ifdef _WIN32
    // Add environmental variables to the registry
    // TODO: Add to HKEY_USERS (i.e. just the user's environment) rather than HKEY_LOCAL_MACHINE
    
    HKEY hkey;
    LPCTSTR key_path = TEXT("HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\Environment");
    LSTATUS l_status;
    l_status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, key_path, 0, KEY_ALL_ACCESS, &hkey);
    if (l_status != ERROR_SUCCESS){
        fprintf(stderr, "Cannot open registry to insert environmental variables\n");
        goto goto_diy_env;
    }
    l_status = RegSetValueEx(hkey,  env_var,  0,  REG_SZ,  (const BYTE*)cfg_pth,  strlen(cfg_pth) + 1);
    RegCloseKey(hkey);
    if (l_status != ERROR_SUCCESS){
        fprintf(stderr, "Cannot set environmental variables in registry\n");
        goto goto_diy_env;
    }
#endif
    
    goto_diy_env:
#ifdef _WIN32
    std::cout << "You need to add the following environmental variable using My Computer > Properties > Advanced > Environmental Variables:" << std::endl;
    std::cout << "Name\tValue" << std::endl;
    std::cout << env_var << "\t" << cfg_pth << std::endl;
#else
    std::cout << "You need to add the following line to your shell profile (such as ~/.bashrc):" << std::endl;
    std::cout << "export " << env_var << "=" << cfg_pth << std::endl;
#endif
    
	wipe_auth(mysql_auth[0],  (uintptr_t)mysql_auth[indx::COUNT] - (uintptr_t)mysql_auth[0]);
    mysql_close(mysql_obj);
}

} // END namespace compsky::mysql
} // END namespace compsky
