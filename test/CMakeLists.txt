add_executable(test_reader test_reader.cpp)
target_link_libraries(test_reader PRIVATE esfa)
if(MSVC)
    target_compile_options(test_reader PRIVATE /UNDEBUG)
else()
    target_compile_options(test_reader PRIVATE -UNDEBUG)
endif()
add_test(
    NAME    test_reader
    COMMAND test_reader
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
