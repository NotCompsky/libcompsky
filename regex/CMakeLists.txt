cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR) # CONTRIBUTIONS WELCOME: Tests of previous/future versions which work or not

add_library(dummylib INTERFACE)

target_include_directories(dummylib INTERFACE 
    $<BUILD_INTERFACE:${INC_DIR}/compsky/regex>
    $<INSTALL_INTERFACE:include>
)

install(
    TARGETS dummylib
    EXPORT CompskyTargets
    PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/compsky/regex"
        COMPONENT dev
)
