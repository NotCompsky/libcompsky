cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR) # CONTRIBUTIONS WELCOME: Tests of previous/future versions which work or not

project(Compsky CXX) # Important that it is set before find_package is called

set(COMPSKY_MAJOR_VERSION 0)
set(COMPSKY_MINOR_VERSION 2)
set(COMPSKY_PATCH_VERSION 0)
set(COMPSKY_VERSION ${COMPSKY_MAJOR_VERSION}.${COMPSKY_MINOR_VERSION}.${COMPSKY_PATCH_VERSION})


include(GNUInstallDirs)


# Ensure symbols are exported for Windows Visual Studio. The alternative is to write the generate export headers commands in the CMakeLists files and import the `<NAME>_export.h`s in all source files.
if(${MSVC})
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "TRUE")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    #set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined -Wl,--enable-runtime-pseudo-reloc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDBUILDING_EXAMPLE_DLL -Wl,--out-implib")
    # as per http://www.mingw.org/wiki/sampleDLL
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

include_directories("${INC_DIR}")

set(PROJECT_TARGETS)
add_subdirectory(asciify)
add_subdirectory(security)
add_subdirectory(mysql)
add_subdirectory(regex)
if(BUILD_PACKAGES)
    set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
    SET(CPACK_RPM_COMPONENT_INSTALL     ON)
    SET(CPACK_DEB_COMPONENT_INSTALL     ON)
    set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)


    if("${BUILD_PACKAGES}" STREQUAL "1")
        set(CPACK_COMPONENTS_ALL asciify asciify-dev mysql mysql-dev regex-dev security security-dev)
        message(STATUS "Note: You can specify the packages to build with -DBUILD_PACKAGES=<package_name>")
    else()
        set(CPACK_COMPONENTS_ALL ${BUILD_PACKAGES})
    endif()


    set(CPACK_COMPONENT_ASCIIFY_DESCRIPTION
        "C++ library for rapid stringification prioritising speed above all")
    set(CPACK_COMPONENT_ASCIIFY_DEV_DESCRIPTION
        "C++ header files")
    set(CPACK_COMPONENT_MYSQL_DESCRIPTION
        "C++ library for interacting with MySQL/MariaDB prioritising speed and usability")
    set(CPACK_COMPONENT_MYSQL_DEV_DESCRIPTION
        "C++ header files")
    set(CPACK_COMPONENT_REGEX_DEV_DESCRIPTION
        "C++ library for pre-processing regex, in particular assigning named groups in a flexible manner")
    set(CPACK_COMPONENT_SECURITY_DESCRIPTION
        "C++ library with secure memset")
    set(CPACK_COMPONENT_SECURITY_DEV_DESCRIPTION
        "C++ header files")

    set(Boost_USE_STATIC_LIBS ON) # Different repos have different libboost-regex versions that different repositories include, just easier to avoid this dependency for packages.
    set(CPACK_DEBIAN_MYSQL_PACKAGE_DEPENDS "compsky-asciify (= ${COMPSKY_VERSION}), compsky-security (= ${COMPSKY_VERSION})")
    set(CPACK_DEBIAN_MYSQL_DEV_PACKAGE_DEPENDS "compsky-asciify-dev (= ${COMPSKY_VERSION}), compsky-security-dev (= ${COMPSKY_VERSION})")

    add_subdirectory(packaging)
endif()

export(TARGETS ${PROJECT_TARGETS} FILE "${PROJECT_BINARY_DIR}/CompskyTargets.cmake")

export(PACKAGE Compsky)

# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(CompskyConfig.cmake.in "${PROJECT_BINARY_DIR}/CompskyConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${COMPSKY_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(CompskyConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CompskyConfig.cmake" @ONLY)
# ... for both
configure_file(CompskyConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/CompskyConfigVersion.cmake" @ONLY)

install(
	FILES
		"${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CompskyConfig.cmake"
		"${PROJECT_BINARY_DIR}/CompskyConfigVersion.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/Compsky/"
	COMPONENT dev
)

install(EXPORT CompskyTargets DESTINATION "${CMAKE_INSTALL_LIBDIR}/Compsky/" COMPONENT dev)

