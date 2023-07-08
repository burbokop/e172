
cmake_policy(SET CMP0076 NEW)

if(NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
    message(FATAL_ERROR "CMAKE_INSTALL_INCLUDEDIR must be defined")
endif()

add_subdirectory(assettools)
add_subdirectory(audio)
add_subdirectory(functional)
add_subdirectory(graphics)
add_subdirectory(math)
add_subdirectory(memcontrol)
add_subdirectory(net)
add_subdirectory(time)
add_subdirectory(utility)

set(INSTALLDIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

target_sources(e172
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/eventhandler.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/eventhandler.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/abstracteventprovider.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/abstracteventprovider.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/additional.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/additional.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/context.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/context.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/entity.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/entity.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/gameapplication.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/gameapplication.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/sharedcontainer.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/sharedcontainer.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/type.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/type.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/variant.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/variant.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/smartenum.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/smartenum.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/object.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/object.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/messagequeue.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/messagequeue.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/debug.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/debug.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/sfinae.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/sfinae.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/typedefs.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/typedefs.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/args.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/args.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/conversion.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/conversion.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/consolecolor.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/consolecolor.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/entitylifetimeobserver.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/entitylifetimeobserver.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/todo.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/todo.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/meta.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/meta.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/traits.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/traits.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/testing.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/testing.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/concepts.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/concepts.h>
PRIVATE
    eventhandler.cpp
    abstracteventprovider.cpp
    additional.cpp
    context.cpp
    entity.cpp
    gameapplication.cpp
    sharedcontainer.cpp
    type.cpp
    variant.cpp
    smartenum.cpp
    object.cpp
    messagequeue.cpp
    debug.cpp
    consolecolor.cpp
    testing.cpp)
