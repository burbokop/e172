set(INSTALLDIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/graphics)

target_sources(
  e172
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/abstractgraphicsprovider.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/abstractgraphicsprovider.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/abstractrenderer.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/abstractrenderer.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/image.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/image.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/imageview.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/imageview.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/textformat.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/textformat.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/shapeformat.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/shapeformat.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/color.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/color.h>
  PRIVATE abstractgraphicsprovider.cpp
          abstractrenderer.cpp
          image.cpp
          imageview.cpp
          textformat.cpp
          shapeformat.cpp
          color.cpp)
