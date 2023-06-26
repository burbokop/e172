find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)
include_directories(${CMAKE_CURRENT_LIST_DIR})

if(E172_STATIC_BUILD)
    add_library(e172 STATIC)
else()
    add_library(e172 SHARED)
endif()

target_sources(e172
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/assettools/abstractassetexecutor.h
    ${CMAKE_CURRENT_LIST_DIR}/src/assettools/assetprovider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/assettools/loadable.h
    ${CMAKE_CURRENT_LIST_DIR}/src/assettools/loadabletemplate.h

    ${CMAKE_CURRENT_LIST_DIR}/src/audio/abstractaudioprovider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/audio/audiochannel.h
    ${CMAKE_CURRENT_LIST_DIR}/src/audio/audiosample.h
    ${CMAKE_CURRENT_LIST_DIR}/src/audio/audioplayer.h

    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/abstractgraphicsprovider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/abstractrenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/image.h
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/imageview.h
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/textformat.h
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/shapeformat.h

    ${CMAKE_CURRENT_LIST_DIR}/src/math/math.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/vector.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/kinematics.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/differentiator.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/averagecalculator.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/colider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/intergrator.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/discretizer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/cellularautomaton.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/line2d.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/physicalobject.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math/matrix.h

    ${CMAKE_CURRENT_LIST_DIR}/src/memcontrol/abstractfactory.h
    ${CMAKE_CURRENT_LIST_DIR}/src/memcontrol/abstractstrategy.h

    ${CMAKE_CURRENT_LIST_DIR}/src/time/deltatimecalculator.h
    ${CMAKE_CURRENT_LIST_DIR}/src/time/elapsedtimer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/time/time.h

    ${CMAKE_CURRENT_LIST_DIR}/src/net/socket.h
    ${CMAKE_CURRENT_LIST_DIR}/src/net/server.h
    ${CMAKE_CURRENT_LIST_DIR}/src/net/networker.h
    ${CMAKE_CURRENT_LIST_DIR}/src/net/gameserver.h
    ${CMAKE_CURRENT_LIST_DIR}/src/net/gameclient.h
    ${CMAKE_CURRENT_LIST_DIR}/src/net/netsync.h
    ${CMAKE_CURRENT_LIST_DIR}/src/net/common.h

    ${CMAKE_CURRENT_LIST_DIR}/src/net/mem/socket.h
    ${CMAKE_CURRENT_LIST_DIR}/src/net/mem/server.h
    ${CMAKE_CURRENT_LIST_DIR}/src/net/mem/networker.h

    ${CMAKE_CURRENT_LIST_DIR}/src/utility/vectorproxy.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/systeminfo.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/animator.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/observer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/cycliciterator.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/ptr.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/dynamiclibrary.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/defer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/lightvector.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/matrixproxy.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/boolean.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/flagparser.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/reflection.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/either.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/option.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/priorityprocedure.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/closableoutputstream.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/signalstreambuffer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/buffer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/io.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/package.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/ringbuf.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/signal.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/callback.h
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/random.h

    ${CMAKE_CURRENT_LIST_DIR}/src/functional/metafunction.h

    ${CMAKE_CURRENT_LIST_DIR}/src/eventhandler.h
    ${CMAKE_CURRENT_LIST_DIR}/src/abstracteventprovider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/additional.h
    ${CMAKE_CURRENT_LIST_DIR}/src/context.h
    ${CMAKE_CURRENT_LIST_DIR}/src/entity.h
    ${CMAKE_CURRENT_LIST_DIR}/src/gameapplication.h
    ${CMAKE_CURRENT_LIST_DIR}/src/sharedcontainer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/type.h
    ${CMAKE_CURRENT_LIST_DIR}/src/variant.h
    ${CMAKE_CURRENT_LIST_DIR}/src/smartenum.h
    ${CMAKE_CURRENT_LIST_DIR}/src/object.h
    ${CMAKE_CURRENT_LIST_DIR}/src/messagequeue.h
    ${CMAKE_CURRENT_LIST_DIR}/src/debug.h
    ${CMAKE_CURRENT_LIST_DIR}/src/sfinae.h
    ${CMAKE_CURRENT_LIST_DIR}/src/typedefs.h
    ${CMAKE_CURRENT_LIST_DIR}/src/args.h
    ${CMAKE_CURRENT_LIST_DIR}/src/conversion.h
    ${CMAKE_CURRENT_LIST_DIR}/src/consolecolor.h
    ${CMAKE_CURRENT_LIST_DIR}/src/entitylifetimeobserver.h
    ${CMAKE_CURRENT_LIST_DIR}/src/todo.h
    ${CMAKE_CURRENT_LIST_DIR}/src/meta.h
    ${CMAKE_CURRENT_LIST_DIR}/src/traits.h
    ${CMAKE_CURRENT_LIST_DIR}/src/testing.h
    ${CMAKE_CURRENT_LIST_DIR}/src/concepts.h
PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/assettools/abstractassetexecutor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/assettools/assetprovider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/assettools/loadable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/assettools/loadabletemplate.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/audio/abstractaudioprovider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/audio/audiochannel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/audio/audiosample.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/audio/audioplayer.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/abstractgraphicsprovider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/abstractrenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/image.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/imageview.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/textformat.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/graphics/shapeformat.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/math/math.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/kinematics.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/differentiator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/averagecalculator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/colider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/intergrator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/line2d.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/physicalobject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math/matrix.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/time/elapsedtimer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/time/time.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/net/networker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/gameserver.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/gameclient.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/utility/systeminfo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/animator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/dynamiclibrary.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/flagparser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/priorityprocedure.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/closableoutputstream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/signalstreambuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utility/random.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/eventhandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/abstracteventprovider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/additional.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/context.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/entity.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/gameapplication.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/sharedcontainer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/type.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/variant.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/smartenum.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/object.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/messagequeue.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/debug.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/consolecolor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/testing.cpp)

if(UNIX)
    target_sources(e172
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src/net/linux/socket.h
        ${CMAKE_CURRENT_LIST_DIR}/src/net/linux/networker.h
        ${CMAKE_CURRENT_LIST_DIR}/src/net/linux/server.h
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/net/linux/socket.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/net/linux/networker.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/net/linux/server.cpp)
else()
    # TODO for windows
endif()

if(UNIX)
    target_link_libraries(e172
        dl
        pthread
        tbb)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmake/utils.cmake)

if(ENABLE_E172_LINT)
    e172_lint_target(e172)
endif()

enable_testing()
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/benches)

install(TARGETS e172 DESTINATION lib)

install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/src/"
    DESTINATION include/e172
    FILES_MATCHING
    PATTERN "*.h"
)
