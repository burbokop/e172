

add_executable(e172_benches
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/variantbenches.cpp
    ${CMAKE_CURRENT_LIST_DIR}/variantbenches.h
)

target_link_libraries(e172_benches
    e172)

if(ENABLE_LINT)
    e172_lint_target(e172_benches)
endif()

enable_testing()
include(CTest)
e172_target_discover_tests(e172_benches)
