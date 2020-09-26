cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)
# NOTE: This is just the earliest version I have tested. Tests of previous/future versions are welcomed.

project(Compsky CXX) # Important that it is set before find_package is called

set(COMPSKY_MAJOR_VERSION 0)
set(COMPSKY_MINOR_VERSION 4)
set(COMPSKY_PATCH_VERSION 0)
set(COMPSKY_VERSION ${COMPSKY_MAJOR_VERSION}.${COMPSKY_MINOR_VERSION}.${COMPSKY_PATCH_VERSION})


include(GNUInstallDirs)


set(INC_DIR "${PROJECT_SOURCE_DIR}/include")


add_library(compsky INTERFACE)
target_compile_features(compsky INTERFACE cxx_std_17)
target_include_directories(compsky INTERFACE 
    $<BUILD_INTERFACE:${INC_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/CompskyConfigVersion.cmake"
    VERSION "${COMPSKY_VERSION}"
    COMPATIBILITY SameMinorVersion
)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/CompskyConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/CompskyConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Compsky"
)
install(FILES "${PROJECT_BINARY_DIR}/CompskyConfigVersion.cmake"
              "${PROJECT_BINARY_DIR}/CompskyConfig.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Compsky")
install(DIRECTORY "${INC_DIR}/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")


add_subdirectory(mysql)
