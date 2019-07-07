cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR) # CONTRIBUTIONS WELCOME: Tests of previous/future versions which work or not

project(Compsky CXX) # Important that it is set before find_package is called

set(COMPSKY_MAJOR_VERSION 0)
set(COMPSKY_MINOR_VERSION 1)
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


if (NOT WHICH_MYSQL_CLIENT)
    find_library(FOUND_MARIADB_CLIENT mariadbclient)
    if (FOUND_MARIADB_CLIENT)
        set(WHICH_MYSQL_CLIENT mariadbclient)
    endif()
endif()
if (NOT WHICH_MYSQL_CLIENT)
    find_library(FOUND_MYSQL_CLIENT mysqlclient)
    if (FOUND_MYSQL_CLIENT)
        set(WHICH_MYSQL_CLIENT mysqlclient)
    endif()
endif()
if (WHICH_MYSQL_CLIENT)
    message(STATUS "WHICH_MYSQL_CLIENT: ${WHICH_MYSQL_CLIENT}")
else()
    message(FATAL_ERROR "Please specify the libmysqlclient equivalent with -DWHICH_MYSQL_CLIENT=...\nFor instance, for Ubuntu/Debian, you would specify -DWHICH_MYSQL_CLIENT=mariadbclient; for others, perhaps, -DWHICH_MYSQL_CLIENT=mysqlclient\nFor Ubuntu/Debian, you can discover this with the command:  apt list --installed | egrep 'lib(mysql|mariadb)client' | grep -v -- -dev | sed 's/^lib//g' | sed -r 's~[0-9]*/.*~~g'")
endif()


# BEGIN src: https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file

if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/Compsky)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# set up include-directories
include_directories("${PROJECT_SOURCE_DIR}")

# Add sub-directories
add_subdirectory(asciify)
add_subdirectory(security)
add_subdirectory(mysql)
add_subdirectory(regex)
if(BUILD_PACKAGES)
    add_subdirectory(packaging)
endif()

# Add all targets to the build-tree export set
export(TARGETS compsky_asciify compsky_security compsky_mysql FILE "${PROJECT_BINARY_DIR}/CompskyTargets.cmake")

# Registers the build-tree with a global CMake-registry
export(PACKAGE Compsky)


# Create the CompskyConfig.cmake and CompskyConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${CMAKE_INSTALL_FULL_INCLUDEDIR}")

# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(CompskyConfig.cmake.in "${PROJECT_BINARY_DIR}/CompskyConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${COMPSKY_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(CompskyConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CompskyConfig.cmake" @ONLY)
# ... for both
configure_file(CompskyConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/CompskyConfigVersion.cmake" @ONLY)

# Install the CompskyConfig.cmake and CompskyConfigVersion.cmake
install(
    FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CompskyConfig.cmake"
    "${PROJECT_BINARY_DIR}/CompskyConfigVersion.cmake"
    DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev
)

# Install the export set for use with the install-tree
install(EXPORT CompskyTargets DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# END src: https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file

