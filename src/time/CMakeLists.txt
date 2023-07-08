
set(INSTALLDIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/time)

target_sources(e172
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/deltatimecalculator.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/deltatimecalculator.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/elapsedtimer.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/elapsedtimer.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/time.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/time.h>
PRIVATE
    elapsedtimer.cpp
    time.cpp)
