cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR) # CONTRIBUTIONS WELCOME: Tests of previous/future versions which work or not

add_library(compsky_regex_dummylib STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/dummy.cpp")

set_target_properties(compsky_regex_dummylib PROPERTIES PUBLIC_HEADER "${INC_DIR}/compsky/regex/named_groups.hpp")

install(
    TARGETS compsky_regex_dummylib
    EXPORT CompskyTargets
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
        COMPONENT shlib
    PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/compsky/regex"
        COMPONENT dev
)
