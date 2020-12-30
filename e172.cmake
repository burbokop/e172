include_directories(${CMAKE_CURRENT_LIST_DIR})

add_library(e172 SHARED

    ${CMAKE_CURRENT_LIST_DIR}/src/assettools/abstractassetexecutor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/assettools/abstractassetexecutor.h
    ${CMAKE_CURRENT_LIST_DIR}/src/assettools/assetprovider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/assettools/assetprovider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/assettools/loadable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/assettools/loadable.h

    ${CMAKE_CURRENT_LIST_DIR}/src/audio/abstractaudioprovider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/audio/abstractaudioprovider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/audio/audiochannel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/audio/audiochannel.h
    ${CMAKE_CURRENT_LIST_DIR}/src/audio/audiosample.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/audio/audiosample.h
    ${CMAKE_CURRENT_LIST_DIR}/src/audio/audioplayer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/audio/audioplayer.h


    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/abstractgraphicsprovider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/abstractgraphicsprovider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/abstractrenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/abstractrenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/image.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/image.h
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/imageview.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/imageview.h

    ${CMAKE_CURRENT_LIST_DIR}/src/math/math.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/math.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/vector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/vector.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/kinematics.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/kinematics.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/differentiator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/differentiator.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/averagecalculator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/averagecalculator.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/colider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/colider.h

    ${CMAKE_CURRENT_LIST_DIR}/src/memcontrol/abstractfactory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/memcontrol/abstractfactory.h
    ${CMAKE_CURRENT_LIST_DIR}/src/memcontrol/abstractstrategy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/memcontrol/abstractstrategy.h

    ${CMAKE_CURRENT_LIST_DIR}/src/time/deltatimecalculator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/time/deltatimecalculator.h
    ${CMAKE_CURRENT_LIST_DIR}/src/time/elapsedtimer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/time/elapsedtimer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/time/time.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/time/time.h

    ${CMAKE_CURRENT_LIST_DIR}/src/utility/vectorproxy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/vectorproxy.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/systeminfo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/systeminfo.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/animator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/animator.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/observer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/observer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/cycliciterator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/cycliciterator.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/ptr.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/ptr.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/dynamiclibrary.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/dynamiclibrary.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/destroysignal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/destroysignal.h

    ${CMAKE_CURRENT_LIST_DIR}/src/abstracteventhandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/abstracteventhandler.h
    ${CMAKE_CURRENT_LIST_DIR}/src/additional.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/additional.h
    ${CMAKE_CURRENT_LIST_DIR}/src/context.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/context.h
    ${CMAKE_CURRENT_LIST_DIR}/src/entity.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/entity.h
    ${CMAKE_CURRENT_LIST_DIR}/src/gameapplication.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/gameapplication.h
    ${CMAKE_CURRENT_LIST_DIR}/src/sharedcontainer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/sharedcontainer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/textformat.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/textformat.h
    ${CMAKE_CURRENT_LIST_DIR}/src/type.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/type.h
    ${CMAKE_CURRENT_LIST_DIR}/src/variant.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/variant.h
    ${CMAKE_CURRENT_LIST_DIR}/src/physicalobject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/physicalobject.h

    ${CMAKE_CURRENT_LIST_DIR}/src/smartenum.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/smartenum.h
    ${CMAKE_CURRENT_LIST_DIR}/src/object.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/object.h
    ${CMAKE_CURRENT_LIST_DIR}/src/messagequeue.h
    ${CMAKE_CURRENT_LIST_DIR}/src/messagequeue.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/functional/metafunction.h
    ${CMAKE_CURRENT_LIST_DIR}/src/functional/metafunction.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/debug.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/debug.h
    ${CMAKE_CURRENT_LIST_DIR}/src/sfinae.h
    ${CMAKE_CURRENT_LIST_DIR}/src/typedefs.h
    ${CMAKE_CURRENT_LIST_DIR}/src/args.h
    ${CMAKE_CURRENT_LIST_DIR}/src/args.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/matrix.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/matrix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/testprovider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/testprovider.cpp
    )

if (UNIX)
target_link_libraries(e172
    dl
    )
endif (UNIX)
