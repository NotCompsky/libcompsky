cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR) # CONTRIBUTIONS WELCOME: Tests of previous/future versions which work or not

find_package(Qt5Widgets)
if(NOT Qt5Widgets_FOUND)
    message(WARNING "Qt5::Widgets not found, so skipping compilation of compsky::qt5")
    return() # Exit this file
endif()

if(1)
    message(WARNING "Skipping compilation of compsky::qt5")
    return() # Exit this file
endif()


# Unused because it is not needed so far. Kept code as example of how to do it (specifically, how to avoid the vtables error)


set(CMAKE_AUTOMOC ON)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
# WARNING: AutoMOC fails to build vtables if the source and header files are in different directories.

# BEGIN: src: https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file #

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_library(compsky_qt5 STATIC ${SRC_DIR}/tagdialog.cpp)
else()
    add_library(compsky_qt5 SHARED ${SRC_DIR}/tagdialog.cpp)
endif()

target_link_libraries(compsky_qt5 PRIVATE Qt5::Widgets)

set_target_properties(compsky_qt5 PROPERTIES PUBLIC_HEADER "${INC_DIR}/compsky/qt5/tagdialog.hpp")

install(
    TARGETS compsky_qt5
    EXPORT CompskyTargets
    RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        COMPONENT bin
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
        COMPONENT shlib
    PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/compsky/qt5"
        COMPONENT dev
)

# END: src: https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file #

return()
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        install(
            # For Windows - install the lib files
            FILES "${CMAKE_BINARY_DIR}/qt5/Release/compsky_qt5.lib"
            DESTINATION "${INSTALL_LIB_DIR}"
        )
    else()
        install(
            # For Windows - install the lib files
            FILES "${CMAKE_BINARY_DIR}/qt5compsky_qt5"
            DESTINATION "${INSTALL_LIB_DIR}"
        )
    endif()
endif()
