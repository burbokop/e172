
set(INSTALLDIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/memcontrol)

target_sources(e172
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/abstractfactory.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/abstractfactory.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/abstractstrategy.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/abstractstrategy.h>
PRIVATE)
