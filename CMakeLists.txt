cmake_minimum_required(VERSION 3.10)

project(graphicsTest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(graphicsTest graphicsTest.c)

target_link_libraries(graphicsTest PRIVATE SDL3::SDL3)