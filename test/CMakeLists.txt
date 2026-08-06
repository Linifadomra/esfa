enable_testing()

set(ESFA_TEST_DIR ${CMAKE_SOURCE_DIR})

add_executable(test_reader test_reader.cpp)
target_link_libraries(test_reader PRIVATE esfa)
add_test(
    NAME    test_reader
    COMMAND test_reader
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
