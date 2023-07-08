
set(INSTALLDIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/assettools)

target_sources(e172
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/abstractassetexecutor.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/abstractassetexecutor.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/assetprovider.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/assetprovider.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/loadable.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/loadable.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/loadabletemplate.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/loadabletemplate.h>
PRIVATE
    abstractassetexecutor.cpp
    assetprovider.cpp
    loadable.cpp
    loadabletemplate.cpp)
