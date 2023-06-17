 
function(e172_target_discover_tests TARGET)
    get_target_property(TARGET_SOURCES ${TARGET} SOURCES)

    foreach(SOURCE_FILE ${TARGET_SOURCES})
        file(READ ${SOURCE_FILE} FILE_CONTENT)
        STRING(REGEX REPLACE ";" "\\\\;" FILE_CONTENT "${FILE_CONTENT}")
        STRING(REGEX REPLACE "\n" ";" FILE_CONTENT "${FILE_CONTENT}")

        foreach(LINE ${FILE_CONTENT})
            string(REGEX MATCH "e172_test\\((.*),[ ]*(.*)\\)" _ ${LINE})
            if(CMAKE_MATCH_1 AND CMAKE_MATCH_2)
                set(TEST_NAME "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}")
                add_test(
                    NAME "${TARGET}.${TEST_NAME}"
                    COMMAND ${TARGET} one ${TEST_NAME}
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
            endif()
        endforeach()
    endforeach()
endfunction()

function(e172_lint_target TARGET)
    get_target_property(TARGET_SOURCES ${TARGET} SOURCES)
    foreach(SOURCE_FILE ${TARGET_SOURCES})
        message("Lint ${SOURCE_FILE}")
            add_custom_command(
                TARGET ${TARGET}
                POST_BUILD
                COMMAND cpplint --output=eclipse ${SOURCE_FILE}
                COMMENT "Linting ${SOURCE_FILE}"
                VERBATIM)
    endforeach()
endfunction()
