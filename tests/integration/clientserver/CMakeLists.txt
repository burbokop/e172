


add_executable(e172_clientserver_integration_test
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/bullet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/bullet.h
    ${CMAKE_CURRENT_LIST_DIR}/shooter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/shooter.h
)

target_link_libraries(e172_clientserver_integration_test
    e172)

enable_testing()
include(CTest)
e172_register_tests(e172_clientserver_integration_test)
