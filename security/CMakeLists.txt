cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR) # CONTRIBUTIONS WELCOME: Tests of previous/future versions which work or not

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# BEGIN: src: https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file #

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_library(compsky_security STATIC ${SRC_DIR}/memset.cpp)
else()
    add_library(compsky_security SHARED ${SRC_DIR}/memset.cpp)
endif()

set_target_properties(compsky_security PROPERTIES PUBLIC_HEADER "${INC_DIR}/compsky/security/memset.hpp")
set_property(TARGET compsky_security PROPERTY CXX_STANDARD 17)

install(
    TARGETS compsky_security
    # Adds the security library to the "export-set"
    EXPORT CompskyTargets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        COMPONENT security
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/compsky/security"
        COMPONENT security-dev
)

# END: src: https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file #

list(APPEND PROJECT_TARGETS "compsky_security")

return()
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        install(
            # For Windows - install the lib files
            FILES "${CMAKE_BINARY_DIR}/security/Release/compsky_security.lib"
	    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
    else()
        install(
            # For Windows - install the lib files
            FILES "${CMAKE_BINARY_DIR}/security/libcompsky_security.dll.a"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
    endif()
endif()
