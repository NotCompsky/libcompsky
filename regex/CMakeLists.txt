cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

add_library(compsky_regex INTERFACE)

target_include_directories(
    compsky_regex
    INTERFACE $<BUILD_INTERFACE:${COMPSKY_INCLUDE_DIR}/compsky/regex>
    $<INSTALL_INTERFACE:include/compsky/regex>
)

install(TARGETS compsky_regex EXPORT Compsky_Targets)

install(DIRECTORY "${INC_DIR}/compsky/regex/" DESTINATION include/compsky COMPONENT dev)
