cmake_minimum_required(VERSION 3.7)

project(e172
    LANGUAGES CXX
    VERSION 0.7.0
    DESCRIPTION "Library provides tools for game gevelopment and api for video/audio output and keyboard input. This lib can be called GAME ENGINE."
    HOMEPAGE_URL "https://github.com/burbokop/e172")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_STATIC_BUILD "Build the static library" OFF)
option(ENABLE_LINT "Enable lint" ON)
option(ENABLE_TESTS "Enable tests" ON)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

include(GNUInstallDirs)

if(ENABLE_STATIC_BUILD)
    add_library(${PROJECT_NAME} STATIC)
else()
    add_library(${PROJECT_NAME} SHARED)
endif()

add_subdirectory(src)

if((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
    target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra)
endif()

if(UNIX)
    target_link_libraries(${PROJECT_NAME}
        dl
        pthread
        tbb)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmake/utils.cmake)

if(ENABLE_LINT)
    e172_lint_target(${PROJECT_NAME})
endif()

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/benches)
endif()

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/src/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
    FILES_MATCHING
    PATTERN "*.h")

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMinorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION
  ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(EXPORT ${PROJECT_NAME}_targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
              "${CMAKE_CURRENT_LIST_DIR}/cmake/utils.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")


set(CPACK_PACKAGE_NAME "libe172")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libtbb2")
set(CPACK_PACKAGE_CONTACT "io.github.burbokop")
include(CPack)
