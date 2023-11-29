cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(HuffmanEncode)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
    HuffNode.cpp
    IntsCompression.cpp
)

set(HEADERS
    HuffNode.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)