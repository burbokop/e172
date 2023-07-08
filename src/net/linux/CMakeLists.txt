
set(INSTALLDIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/net/linux)

target_sources(e172
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/socket.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/socket.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/server.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/server.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/networker.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/networker.h>
PRIVATE
    socket.cpp
    networker.cpp
    server.cpp)
