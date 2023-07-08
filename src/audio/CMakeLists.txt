
set(INSTALLDIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/audio)

target_sources(e172
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/abstractaudioprovider.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/abstractaudioprovider.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/audiochannel.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/audiochannel.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/audiosample.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/audiosample.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/audioplayer.h>
    $<INSTALL_INTERFACE:${INSTALLDIR}/audioplayer.h>
PRIVATE
    abstractaudioprovider.cpp
    audiochannel.cpp
    audiosample.cpp
    audioplayer.cpp)
