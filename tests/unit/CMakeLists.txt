


add_executable(e172_unit_tests
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/additionalspec.cpp
    ${CMAKE_CURRENT_LIST_DIR}/additionalspec.h
    ${CMAKE_CURRENT_LIST_DIR}/variantspec.cpp
    ${CMAKE_CURRENT_LIST_DIR}/variantspec.h
    ${CMAKE_CURRENT_LIST_DIR}/priorityprocedurespec.cpp
    ${CMAKE_CURRENT_LIST_DIR}/priorityprocedurespec.h
    ${CMAKE_CURRENT_LIST_DIR}/bufferspec.h
    ${CMAKE_CURRENT_LIST_DIR}/bufferspec.cpp
    ${CMAKE_CURRENT_LIST_DIR}/packagespec.h
    ${CMAKE_CURRENT_LIST_DIR}/packagespec.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ringbufspec.h
    ${CMAKE_CURRENT_LIST_DIR}/ringbufspec.cpp
    ${CMAKE_CURRENT_LIST_DIR}/typespec.h
    ${CMAKE_CURRENT_LIST_DIR}/typespec.cpp
    ${CMAKE_CURRENT_LIST_DIR}/flagparserspec.h
    ${CMAKE_CURRENT_LIST_DIR}/flagparserspec.cpp)

target_link_libraries(e172_unit_tests
    e172)

if(ENABLE_LINT)
    e172_lint_target(e172_unit_tests)
endif()

enable_testing()
include(CTest)
e172_target_discover_tests(e172_unit_tests)
