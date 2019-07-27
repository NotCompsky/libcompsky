cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR) # CONTRIBUTIONS WELCOME: Tests of previous/future versions which work or not

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")


find_library(FINDLIB_mariadbclient		mariadbclient)
find_library(FINDLIB_mysqlclient		mysqlclient)
find_library(FINDLIB_mariadb-connector-c	mariadb-connector-c)
find_library(FINDLIB_mysql-connector-c		mysql-connector-c)

foreach(sqlconn mariadbclient mysqlclient mariadb-connector-c mysql-connector-c)
	if (NOT WHICH_MYSQL_CLIENT)
		if (FINDLIB_${sqlconn})
			set(WHICH_MYSQL_CLIENT "${sqlconn}")
		endif()
	endif()
endforeach(sqlconn)

if (WHICH_MYSQL_CLIENT)
    message(STATUS "WHICH_MYSQL_CLIENT: ${WHICH_MYSQL_CLIENT}")
else()
    message(FATAL_ERROR "Please specify the libmysqlclient equivalent with -DWHICH_MYSQL_CLIENT=...\nFor instance, for Ubuntu/Debian, you would specify -DWHICH_MYSQL_CLIENT=mariadbclient; for others, perhaps, -DWHICH_MYSQL_CLIENT=mysqlclient\nFor Ubuntu/Debian, you can discover this with the command:  apt list --installed | grep -e 'lib(mysql|mariadb)client' | grep -v -- -dev | sed 's/^lib//g' | sed -r 's~[0-9]*/.*~~g'\nFor MacOS, it is probably mysql-connector-c")
endif()


# BEGIN: src: https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file #

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_library(compsky_mysql STATIC "${SRC_DIR}/mysql.cpp")
    add_library(compsky_mysql_create_config STATIC "${SRC_DIR}/create_config.cpp")
    target_compile_definitions(compsky_mysql_create_config PRIVATE COMPILING_STATIC_LIB)
else()
    add_library(compsky_mysql SHARED "${SRC_DIR}/mysql.cpp")
    add_library(compsky_mysql_create_config SHARED "${SRC_DIR}/create_config.cpp")
endif()

target_link_libraries(compsky_mysql PUBLIC "${WHICH_MYSQL_CLIENT}") # Interface needed for header-only parts
target_link_libraries(compsky_mysql PRIVATE compsky_asciify compsky_security)
target_link_libraries(compsky_mysql_create_config PRIVATE compsky_asciify compsky_mysql)

set_target_properties(compsky_mysql PROPERTIES PUBLIC_HEADER "${INC_DIR}/compsky/mysql/mysql.hpp;${INC_DIR}/compsky/mysql/query.hpp;${INC_DIR}/compsky/mysql/mysql.h")
set_target_properties(compsky_mysql_create_config PROPERTIES PUBLIC_HEADER "${INC_DIR}/compsky/mysql/create_config.hpp")

set_property(TARGET compsky_mysql PROPERTY CXX_STANDARD 17)
set_property(TARGET compsky_mysql_create_config PROPERTY CXX_STANDARD 17)

install(
    TARGETS compsky_mysql compsky_mysql_create_config
    # Adds the mysql library to the "export-set"
    EXPORT CompskyTargets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        COMPONENT mysql
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/compsky/mysql"
        COMPONENT mysql-dev
)

# END: src: https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file #

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows" AND NOT ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(compsky_mysql PRIVATE z secur32 ssl crypto crypt32 ws2_32)
    target_link_libraries(compsky_mysql_create_config PRIVATE secur32 ws2_32)
endif()

include(CheckIncludeFiles)
check_include_files(mysql/mysql.h   MYSQL_IS_UNDER_MYSQL_DIR)
check_include_files(mariadb/mysql.h MYSQL_IS_UNDER_MARIADB_DIR)
check_include_files(mysql.h         MYSQL_IS_NOT_UNDER_DIR)
if(MYSQL_UNDER_DIR_OVERRIDE)
elseif(
    (MYSQL_IS_NOT_UNDER_DIR     AND NOT (MYSQL_IS_UNDER_MARIADB_DIR AND MYSQL_IS_UNDER_MYSQL_DIR)) OR
    (MYSQL_IS_UNDER_MARIADB_DIR AND NOT (MYSQL_IS_NOT_UNDER_DIR     AND MYSQL_IS_UNDER_MYSQL_DIR)) OR
    (MYSQL_IS_UNDER_MYSQL_DIR   AND NOT (MYSQL_IS_NOT_UNDER_DIR     AND MYSQL_IS_UNDER_MARIADB_DIR))
)
elseif(MYSQL_IS_NOT_UNDER_DIR OR MYSQL_IS_UNDER_MYSQL_DIR OR MYSQL_IS_UNDER_MARIADB_DIR)
else()
    message(FATAL_ERROR "Cannot include mysql.h - please manually define -DMYSQL_UNDER_DIR=<path to MySQL root directory> and -DMYSQL_UNDER_DIR_OVERRIDE=1")
endif()

configure_file("${INC_DIR}/compsky/mysql/mysql.h.in" "${INC_DIR}/compsky/mysql/mysql.h")
