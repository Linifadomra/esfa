# == ESFA - CMake Build Configuration ==

cmake_minimum_required(VERSION 3.25)
project(esfa VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# == Options ==
option(BUILD_SHARED_LIBS "Build ESFA as a shared library" OFF)
option(ESFA_BUILD_TESTS  "Build ESFA tests"               OFF)

if(BUILD_SHARED_LIBS)
    set(ESFA_LIB_TYPE STATIC)
else()
    set(ESFA_LIB_TYPE SHARED)
endif()

# == Includes / Building ==

set(ESFA_PUBLIC_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(ESFA_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Processing

add_library(processing ${ESFA_LIB_TYPE}
    # binary
    ${ESFA_SRC}/processing/binary/reader.cpp
    ${ESFA_SRC}/processing/binary/writer.cpp
    # stream
    ${ESFA_SRC}/processing/stream/file_stream.cpp
)

target_include_directories(processing PUBLIC ${ESFA_PUBLIC_INCLUDES} PRIVATE ${ESFA_SRC}/processing)

# Interface

add_library(interface ${ESFA_LIB_TYPE}
    ${ESFA_SRC}/interface/registry.cpp
)

target_include_directories(interface PUBLIC ${ESFA_PUBLIC_INCLUDES} PRIVATE ${ESFA_SRC}/interface)

# esfa lib

add_library(esfa ${ESFA_SRC}/esfa.cpp)
target_include_directories(esfa PUBLIC ${ESFA_PUBLIC_INCLUDES})
target_link_libraries(esfa PUBLIC processing interface)
set_target_properties(esfa PROPERTIES
VERSION   ${PROJECT_VERSION}
SOVERSION ${PROJECT_VERSION_MAJOR})

# binary / tests

if(ESFA_BUILD_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
