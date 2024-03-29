set(INSTALLDIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/math)

target_sources(
  e172
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/math.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/math.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/vector.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/vector.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/kinematics.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/kinematics.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/differentiator.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/differentiator.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/averagecalculator.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/averagecalculator.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/colider.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/colider.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/intergrator.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/intergrator.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/discretizer.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/discretizer.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/cellularautomaton.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/cellularautomaton.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/line2d.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/line2d.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/physicalobject.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/physicalobject.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/matrix.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/matrix.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/rational.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/rational.h>
  PRIVATE math.cpp
          kinematics.cpp
          differentiator.cpp
          averagecalculator.cpp
          colider.cpp
          intergrator.cpp
          line2d.cpp
          physicalobject.cpp
          matrix.cpp)
