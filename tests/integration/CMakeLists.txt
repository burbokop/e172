

if(UNIX)
    enable_testing()
    add_subdirectory(clientserver)
else()
    # TODO for windows
endif()
