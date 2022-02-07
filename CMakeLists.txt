cmake_minimum_required(VERSION 3.7)
project(e172 LANGUAGES CXX VERSION 0.6.6)


set(CPACK_PACKAGE_NAME "libe172")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libtbb2")
set(CPACK_PACKAGE_CONTACT "io.github.burbokop")

include(CPack)


message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
message("CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(e172.cmake)