#include "compsky/mysql/create_config.hpp"



#include <stdio.h> // for fprintf
#include <iostream> // for std::cout

#include <compsky/asciify/flags.hpp>
#include <compsky/mysql/query.hpp>

#ifdef _WIN32
  #include <windows.h>
#endif


namespace compsky {
namespace asciify {
    char* BUF = (char*)malloc(1024);
    size_t BUF_INDX = 0;
}


namespace mysql {

#ifdef COMPILING_STATIC_LIB
extern MYSQL OBJ;
extern char* MYSQL_AUTH[6];
#else
MYSQL OBJ;
char* MYSQL_AUTH[6];
#endif

MYSQL_RES* RES1;
MYSQL_ROW ROW1;


char* AUTH_PTR;

void ef_reed(){
    char c;
    while((c = fgetc(stdin))){
        if (c != '\n'){
            *AUTH_PTR = c;
            ++AUTH_PTR;
        } else return;
    }
}


void create_config(const char* stmts,  const char* env_var){
    std::cout << "* MySQL Configuration *" << std::endl;
    
    std::cout << "Absolute path to save the config file to: ";
    char cfg_pth[1024];
    {
    char c;
    for(auto i = 0;  (c = fgetc(stdin));  ++i){
        if (c != '\n')
            cfg_pth[i] = c;
        else break;
    }
    }
    
    char auth[4096];
    memcpy(auth, "HOST: ", 6);
    char* AUTH_PTR_ENDS[6];
    AUTH_PTR = auth + 6;
    MYSQL_AUTH[0] = AUTH_PTR;
    auto i = 0;
    
    std::cout << "Host (localhost if it is on this machine): ";
    ef_reed();
    AUTH_PTR_ENDS[i] = AUTH_PTR;
    memcpy(AUTH_PTR, "\nPATH: ", 7);
    AUTH_PTR += 7;
    MYSQL_AUTH[++i] = AUTH_PTR;
    
    bool is_localhost = (strncmp(MYSQL_AUTH[i-1], "localhost", strlen("localhost")) == 0);
    
  #ifndef _WIN32
    if (is_localhost){
        FILE* proc = popen("mysql_config --socket", "r");
        AUTH_PTR += fread(AUTH_PTR, 1, 1024, proc) - 1; // Overwrite trailing newline
        pclose(proc);
    } else {
  #endif
        std::cout << "Path: ";
        ef_reed();
  #ifndef _WIN32
    }
  #endif
    AUTH_PTR_ENDS[i] = AUTH_PTR;
    memcpy(AUTH_PTR, "\nUSER: ", 7);
    AUTH_PTR += 7;
    MYSQL_AUTH[++i] = AUTH_PTR;
    
    std::cout << "Username: ";
    ef_reed();
    AUTH_PTR_ENDS[i] = AUTH_PTR;
    memcpy(AUTH_PTR, "\nPWRD: ", 7);
    AUTH_PTR += 7;
    MYSQL_AUTH[++i] = AUTH_PTR;
    
    std::cout << "Corresponding password: ";
    ef_reed();
    AUTH_PTR_ENDS[i] = AUTH_PTR;
    memcpy(AUTH_PTR, "\nDBNM: ", 7);
    AUTH_PTR += 7;
    MYSQL_AUTH[++i] = AUTH_PTR;
    
    std::cout << "Database name: ";
    ef_reed();
    AUTH_PTR_ENDS[i] = AUTH_PTR;
    memcpy(AUTH_PTR, "\nPORT: ", 7);
    AUTH_PTR += 7;
    MYSQL_AUTH[++i] = AUTH_PTR;
    
    if (is_localhost){
        *AUTH_PTR = '0';
        ++AUTH_PTR;
    } else {
        std::cout << "Port number: ";
        ef_reed();
    }
    AUTH_PTR_ENDS[i] = AUTH_PTR;
    *AUTH_PTR = '\n'; // Important that there is a trailing newline
    
    FILE* cfg = fopen(cfg_pth, "wb");
    fwrite(auth,  1,  (uintptr_t)AUTH_PTR + 1 - (uintptr_t)auth,  cfg);
    fclose(cfg);
    
    for (auto j = 0;  j < 6;  ++j)
        AUTH_PTR_ENDS[j][0] = 0;
    for (auto j = 0;  j < 6;  ++j)
        printf("MYSQL_AUTH[%d] = %s\n", j, MYSQL_AUTH[j]);
    
    /* Now to login to the MySQL database with the root user */
    
    const char* username = MYSQL_AUTH[2]; // User to grant permissions to
    const char* password = MYSQL_AUTH[3]; // His password
    
    std::cout << "MySQL admin username: ";
    ef_reed();
    MYSQL_AUTH[2] = AUTH_PTR;
    
    const uintptr_t AUTH_PTR_before = (uintptr_t)AUTH_PTR;
    *AUTH_PTR = 0; // So we can write it out here:
    std::cout << "MySQL admin password (leave blank to use system socket authentication - i.e. if you can login to MySQL as `" << MYSQL_AUTH[2] << "` without a password): ";
    ef_reed();
    if (AUTH_PTR_before == (uintptr_t)AUTH_PTR)
        MYSQL_AUTH[3] = nullptr;
    else
        MYSQL_AUTH[3] = AUTH_PTR;
    
    const char* db_name = MYSQL_AUTH[4];
    MYSQL_AUTH[4] = nullptr; // Haven't created the database yet
    
    
    login_from_auth();
    
    exec("CREATE DATABASE IF NOT EXISTS `", db_name, "`");
    
    mysql_select_db(&OBJ, db_name); // Set as current database
    
    /* The following is an alternative to using `mysql_set_server_option(&OBJ, MYSQL_OPTION_MULTI_STATEMENTS_ON);`
     * The reason it is used is that it is easier to debug the SQL commands individually
     */
    char* stmt = const_cast<char*>(stmts);
    for (char* itr = stmt;  *itr != 0;  ++itr){
        if (unlikely(*itr == ';')){
            exec_buffer(stmt,  (uintptr_t)itr - (uintptr_t)stmt);
            stmt = itr + 1;
        }
    }
    
    constexpr static const asciify::flag::Escape esc;
    
    if (is_localhost){
        exec("CREATE USER IF NOT EXISTS `", esc, '`', username, "`@`localhost` IDENTIFIED BY \"", esc, '"', password, "\"");
        exec("GRANT SELECT, INSERT, UPDATE ON ", db_name, ".* TO `", esc, '`', username, "`@`localhost`");
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
    
    exit_mysql();
}

} // END namespace compsky::mysql
} // END namespace compsky
