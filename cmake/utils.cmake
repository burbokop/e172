 
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
        add_custom_command(
            TARGET ${TARGET}
            POST_BUILD
            COMMAND cpplint --output=eclipse ${SOURCE_FILE}
            COMMENT "Linting ${SOURCE_FILE}"
            VERBATIM)
    endforeach()
endfunction()



function(e172_copy_target TARGET DST_DIR)
    get_target_property(TARGET_SOURCES ${TARGET} SOURCES)
    get_target_property(SOURCE_DIR ${TARGET} SOURCE_DIR)

    foreach(SRC_FILE_PATH ${TARGET_SOURCES})
        file(RELATIVE_PATH REL_SRC_PATH ${SOURCE_DIR} ${SRC_FILE_PATH})
        file(REAL_PATH ${REL_SRC_PATH} DST_FILE_PATH BASE_DIRECTORY ${DST_DIR})

        add_custom_command(
            TARGET ${TARGET}
            POST_BUILD
            BYPRODUCTS "${DST_FILE_PATH}"
            COMMAND cmake -E copy "${SRC_FILE_PATH}" "${DST_FILE_PATH}"
            DEPENDS "${SRC_FILE_PATH}"
            COMMENT "Copiyng ${SRC_FILE_PATH} -> ${DST_FILE_PATH}"
            VERBATIM)
    endforeach()
endfunction()
