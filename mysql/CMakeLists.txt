cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR) # CONTRIBUTIONS WELCOME: Tests of previous/future versions which work or not

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

find_library(FINDLIB_mariadbclient		mariadbclient)
find_library(FINDLIB_mysqlclient		mysqlclient)
find_library(FINDLIB_mariadb-connector-c	mariadb-connector-c)
find_library(FINDLIB_mysql-connector-c		mysql-connector-c)

foreach(sqlconn mariadbclient mysqlclient mariadb-connector-c mysql-connector-c)
# WARNING: WHICH_MYSQL_CLIENT always (falsely) gives mariadbclient
	if (NOT WHICH_MYSQL_CLIENT)
		if (FINDLIB_${sqlconn})
			set(WHICH_MYSQL_CLIENT "${sqlconn}")
		endif()
	endif()
endforeach(sqlconn)

if (WHICH_MYSQL_CLIENT)
    message(STATUS "WHICH_MYSQL_CLIENT: ${WHICH_MYSQL_CLIENT}")
else()
    message(WARNING "Compiling without compsky::mysql")
    message(STATUS "To compile with compsky::mysql, install a client dev library.")
    message(STATUS "If you already have, and it wasn't detected by cmake, specify the library equivalent with -DWHICH_MYSQL_CLIENT=...\nFor instance, for Ubuntu/Debian, you would specify -DWHICH_MYSQL_CLIENT=mariadbclient; for others, perhaps, -DWHICH_MYSQL_CLIENT=mysqlclient\nFor Ubuntu/Debian, you can discover this with the command:  apt list --installed | grep -e 'lib(mysql|mariadb)client' | grep -v -- -dev | sed 's/^lib//g' | sed -r 's~[0-9]*/.*~~g'\nFor MacOS, it is probably mysql-connector-c")
    return()
endif()


# BEGIN: src: https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file #

add_library(compsky_mysql INTERFACE)
add_library(compsky_mysql_create_config STATIC "${SRC_DIR}/create_config.cpp")

target_link_libraries(compsky_mysql INTERFACE "${WHICH_MYSQL_CLIENT}") # Interface needed for header-only parts
target_link_libraries(compsky_mysql_create_config PRIVATE compsky)
target_link_libraries(compsky_mysql_create_config PUBLIC "${WHICH_MYSQL_CLIENT}")

set_target_properties(compsky_mysql_create_config PROPERTIES PUBLIC_HEADER "${INC_DIR}/compsky/mysql/create_config.hpp")

target_compile_features(compsky_mysql INTERFACE cxx_std_17)
set_property(TARGET compsky_mysql_create_config PROPERTY CXX_STANDARD 17)

target_include_directories(compsky_mysql INTERFACE 
	$<BUILD_INTERFACE:${INC_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(
    TARGETS compsky_mysql compsky_mysql_create_config
    # Adds the mysql library to the "export-set"
    EXPORT CompskyTargets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        COMPONENT mysql
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/compsky/mysql"
        COMPONENT mysql-dev
)

install(
    TARGETS compsky_mysql
    EXPORT CompskyTargets
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/compsky/mysql"
    COMPONENT mysql-dev
)

# END: src: https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file #

include(CheckIncludeFiles)
if(NOT MYSQL_UNDER_DIR_OVERRIDE)
	check_include_files(mysql/mysql.h   MYSQL_IS_UNDER_MYSQL_DIR)
	check_include_files(mariadb/mysql.h MYSQL_IS_UNDER_MARIADB_DIR)
	check_include_files(mysql.h         MYSQL_IS_NOT_UNDER_DIR)
	
	if(
		(MYSQL_IS_NOT_UNDER_DIR     AND NOT (MYSQL_IS_UNDER_MARIADB_DIR AND MYSQL_IS_UNDER_MYSQL_DIR)) OR
		(MYSQL_IS_UNDER_MARIADB_DIR AND NOT (MYSQL_IS_NOT_UNDER_DIR     AND MYSQL_IS_UNDER_MYSQL_DIR)) OR
		(MYSQL_IS_UNDER_MYSQL_DIR   AND NOT (MYSQL_IS_NOT_UNDER_DIR     AND MYSQL_IS_UNDER_MARIADB_DIR))
	)
	elseif(MYSQL_IS_NOT_UNDER_DIR OR MYSQL_IS_UNDER_MYSQL_DIR OR MYSQL_IS_UNDER_MARIADB_DIR)
	else()
		message(FATAL_ERROR "Cannot include mysql.h - please manually define -DMYSQL_UNDER_DIR_OVERRIDE=1, and provide one of MYSQL_IS_UNDER_MARIADB_DIR=1 or MYSQL_IS_UNDER_MYSQL_DIR=1, and provide MYSQL_IS_NOT_UNDER_DIR=1 if the mysql.h header file is not underneath mysql/ or mariadb/ include directories (i.e. is in the root include directory)")
	endif()
endif()

configure_file("${INC_DIR}/compsky/mysql/mysql.h.in" "${INC_DIR}/compsky/mysql/mysql.h")
