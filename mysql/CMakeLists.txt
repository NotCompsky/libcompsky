cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR) # CONTRIBUTIONS WELCOME: Tests of previous/future versions which work or not

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(IS_DIRECTORY "C:/Program Files/MySQL/MySQL Server 8.0")
    set(WIN_MYSQL_DIR "C:/Program Files/MySQL/MySQL Server 8.0")
elseif(IS_DIRECTORY "D:/Program Files/MySQL/MySQL Server 8.0")
    set(WIN_MYSQL_DIR "D:/Program Files/MySQL/MySQL Server 8.0")
elseif(IS_DIRECTORY "E:/Program Files/MySQL/MySQL Server 8.0")
    set(WIN_MYSQL_DIR "E:/Program Files/MySQL/MySQL Server 8.0")
elseif(IS_DIRECTORY "F:/Program Files/MySQL/MySQL Server 8.0")
    set(WIN_MYSQL_DIR "F:/Program Files/MySQL/MySQL Server 8.0")
elseif(IS_DIRECTORY "G:/Program Files/MySQL/MySQL Server 8.0")
    set(WIN_MYSQL_DIR "G:/Program Files/MySQL/MySQL Server 8.0")
elseif(IS_DIRECTORY "H:/Program Files/MySQL/MySQL Server 8.0")
    set(WIN_MYSQL_DIR "H:/Program Files/MySQL/MySQL Server 8.0")
endif()

if(NOT "${WIN_MYSQL_DIR} " STREQUAL " ") # Trailing space is important - string equality check fails for two empty strings
    include_directories("${WIN_MYSQL_DIR}/include")
    link_directories("${WIN_MYSQL_DIR}/lib")
    message(STATUS "Using MySQL directory: ${WIN_MYSQL_DIR}")
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

target_link_libraries(compsky_mysql PRIVATE mysqlclient compsky_asciify compsky_security)
target_link_libraries(compsky_mysql_create_config PRIVATE compsky_asciify compsky_mysql)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../security/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../asciify/include")

set_target_properties(compsky_mysql PROPERTIES PUBLIC_HEADER "${INC_DIR}/compsky/mysql/mysql.hpp;${INC_DIR}/compsky/mysql/query.hpp;${INC_DIR}/compsky/mysql/mysql.h")
set_target_properties(compsky_mysql_create_config PROPERTIES PUBLIC_HEADER "${INC_DIR}/compsky/mysql/create_config.hpp")

set_property(TARGET compsky_mysql PROPERTY CXX_STANDARD 17)
set_property(TARGET compsky_mysql_create_config PROPERTY CXX_STANDARD 17)

install(
    TARGETS compsky_mysql compsky_mysql_create_config
    # Adds the mysql library to the "export-set"
    EXPORT CompskyTargets
    RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        COMPONENT bin
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
        COMPONENT shlib
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        COMPONENT slib
    PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/compsky/mysql"
        COMPONENT dev
)

# END: src: https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file #

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows" AND NOT ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(compsky_mysql PRIVATE z secur32 ssl crypto crypt32 ws2_32)
    target_link_libraries(compsky_mysql_create_config PRIVATE secur32 ws2_32)
endif()

include(CheckCXXSourceCompiles)
check_cxx_source_compiles("#include <mysql/mysql.h>" MYSQL_IS_UNDER_DIR)
check_cxx_source_compiles("#include <mysql.h>" MYSQL_IS_NOT_UNDER_DIR)
if(MYSQL_UNDER_DIR_OVERRIDE)
elseif(NOT MYSQL_IS_UNDER_DIR AND NOT MYSQL_IS_NOT_UNDER_DIR)
    message(FATAL_ERROR "Cannot include mysql.h - please manually define -DMYSQL_UNDER_DIR=... and -DMYSQL_UNDER_DIR_OVERRIDE=1")
    set(MYSQL_UNDER_DIR 1)
elseif(MYSQL_IS_UNDER_DIR)
    set(MYSQL_UNDER_DIR 1)
else()
    unset(MYSQL_UNDER_DIR)
endif()

configure_file("${INC_DIR}/compsky/mysql/mysql.h.in" "${INC_DIR}/compsky/mysql/mysql.h")
