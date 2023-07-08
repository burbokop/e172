
if(UNIX)
    add_subdirectory(linux)
else()
    # TODO for windows
endif()

add_subdirectory(mem)

set(INSTALLDIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/net)

target_sources(e172
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/socket.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/socket.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/server.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/server.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/networker.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/networker.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/gameserver.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/gameserver.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/gameclient.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/gameclient.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/netsync.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/netsync.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/common.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/common.h>
PRIVATE
    networker.cpp
    gameserver.cpp
    gameclient.cpp)
