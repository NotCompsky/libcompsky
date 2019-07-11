cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR) # CONTRIBUTIONS WELCOME: Tests of previous/future versions which work or not

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# BEGIN: src: https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file #

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_library(compsky_asciify STATIC "${SRC_DIR}/types.cpp" "${SRC_DIR}/base.cpp")
    target_compile_definitions(compsky_asciify PRIVATE COMPILING_STATIC_LIB)
else()
    add_library(compsky_asciify SHARED "${SRC_DIR}/types.cpp" "${SRC_DIR}/base.cpp")
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

set(headers
	"${INC_DIR}/compsky/asciify/utils.hpp"
	"${INC_DIR}/compsky/asciify/types.hpp"
	"${INC_DIR}/compsky/asciify/flags.hpp"
	"${INC_DIR}/compsky/asciify/base.hpp"
	"${INC_DIR}/compsky/asciify/asciify_h.hpp"
	"${INC_DIR}/compsky/asciify/asciify.hpp"
	"${INC_DIR}/compsky/asciify/print.hpp"
	"${INC_DIR}/compsky/asciify/time_h.hpp"
	"${INC_DIR}/compsky/asciify/time.hpp"
	"${INC_DIR}/compsky/asciify/concat_h.hpp"
	"${INC_DIR}/compsky/asciify/concat.hpp"
)
set_target_properties(compsky_asciify
	PROPERTIES PUBLIC_HEADER "${headers}"
)

set_property(TARGET compsky_asciify PROPERTY CXX_STANDARD 17)

find_package(Qt5Widgets)
if(Qt5Widgets_FOUND)
    include_directories(${Qt5Widgets_INCLUDE_DIRS})
    link_directories(${Qt5Widgets_LIB_DIRS})
    target_compile_definitions(compsky_asciify PRIVATE QT_GUI_LIB=1)
    target_compile_definitions(compsky_asciify PRIVATE QT_NO_VERSION_TAGGING=1)
else()
    message(WARNING "Qt5Widgets not found, so not compiling with QString support in compsky::asciify")
endif()

install(
    TARGETS compsky_asciify
    # Adds the asciify library to the "export-set"
    EXPORT CompskyTargets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        COMPONENT lib
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/compsky/asciify"
        COMPONENT dev
)

# END: src: https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file #

return()
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        install(
            # For Windows - install the lib files
            FILES "${CMAKE_BINARY_DIR}/asciify/Release/compsky_asciify.lib"
	    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
    else()
        install(
            # For Windows - install the lib files
            FILES "${CMAKE_BINARY_DIR}/asciify/libcompsky_asciify.dll.a"
	    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
    endif()
endif()
