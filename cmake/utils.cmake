 
function(e172_register_tests TARGET)
    get_target_property(TARGET_SOURCES ${TARGET} SOURCES)

    foreach(SOURCE_FILE ${TARGET_SOURCES})
        file(READ ${SOURCE_FILE} FILE_CONTENT)
        STRING(REGEX REPLACE ";" "\\\\;" FILE_CONTENT "${FILE_CONTENT}")
        STRING(REGEX REPLACE "\n" ";" FILE_CONTENT "${FILE_CONTENT}")

        foreach(LINE ${FILE_CONTENT})
            string(REGEX MATCH "e172_test\\((.*),[ ]*(.*)\\)" _ ${LINE})
            if(CMAKE_MATCH_1 AND CMAKE_MATCH_2)
                set(TEST_NAME "${CMAKE_MATCH_1}:${CMAKE_MATCH_2}")
                message("test registered: ${TARGET}:${TEST_NAME}")
                add_test(NAME "${TARGET}:${TEST_NAME}" COMMAND ${TARGET} one ${TEST_NAME})
            endif()
        endforeach()
    endforeach()
endfunction()

