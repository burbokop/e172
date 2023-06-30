


add_executable(e172_clientserver_integration_test
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/bullet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/bullet.h
    ${CMAKE_CURRENT_LIST_DIR}/shooter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/shooter.h
)

target_link_libraries(e172_clientserver_integration_test
    e172)

if(ENABLE_LINT)
    e172_lint_target(e172_clientserver_integration_test)
endif()

enable_testing()
include(CTest)
e172_target_discover_tests(e172_clientserver_integration_test)
