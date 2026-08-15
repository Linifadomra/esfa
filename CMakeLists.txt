# == ESFA - CMake Build Configuration ==

cmake_minimum_required(VERSION 3.25)
project(esfa VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# == Options ==
option(BUILD_SHARED_LIBS "Build ESFA as a shared library" OFF)
option(ESFA_BUILD_TESTS  "Build ESFA tests"               OFF)

if(BUILD_SHARED_LIBS)
    set(ESFA_LIB_TYPE SHARED)
else()
    set(ESFA_LIB_TYPE STATIC)
endif()

# == Includes / Building ==

set(ESFA_PUBLIC_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(ESFA_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# == Compiler Warnings ==

if(MSVC)
    set(ESFA_WARNING_FLAGS
        /W4
        /permissive-
        /w14242
        /w14254
        /w14263
        /w14265
        /w14287
        /we4289
        /w14296
        /w14311
        /w14545
        /w14546
        /w14547
        /w14549
        /w14555
        /w14619
        /w14640
        /w14826
        /w14905
        /w14906
        /w14928
    )
else()
    set(ESFA_WARNING_FLAGS
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
        -Wimplicit-fallthrough
    )
endif()

# Processing

add_library(processing ${ESFA_LIB_TYPE}
    # binary
    ${ESFA_SRC}/processing/binary/reader.cpp
    ${ESFA_SRC}/processing/binary/writer.cpp
    # stream
    ${ESFA_SRC}/processing/stream/file_stream.cpp
    ${ESFA_SRC}/processing/stream/bounded_stream.cpp
    ${ESFA_SRC}/processing/stream/memory_stream.cpp
)

target_include_directories(processing PUBLIC ${ESFA_PUBLIC_INCLUDES} PRIVATE ${ESFA_SRC}/processing)
target_compile_options(processing PRIVATE ${ESFA_WARNING_FLAGS})

# Interface

add_library(interface ${ESFA_LIB_TYPE}
    ${ESFA_SRC}/interface/registry.cpp
)

target_include_directories(interface PUBLIC ${ESFA_PUBLIC_INCLUDES} PRIVATE ${ESFA_SRC}/interface)
target_compile_options(interface PRIVATE ${ESFA_WARNING_FLAGS})

# esfa lib

add_library(esfa ${ESFA_SRC}/esfa.cpp)
target_include_directories(esfa PUBLIC ${ESFA_PUBLIC_INCLUDES})
target_link_libraries(esfa PUBLIC processing interface)
target_compile_options(esfa PRIVATE ${ESFA_WARNING_FLAGS})
set_target_properties(esfa PROPERTIES
VERSION   ${PROJECT_VERSION}
SOVERSION ${PROJECT_VERSION_MAJOR})


# binary / tests

if(ESFA_BUILD_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
