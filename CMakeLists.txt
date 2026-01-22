cmake_minimum_required(VERSION 3.22)

project(OilProject VERSION 0.3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(oilc)
add_library(oillib STATIC)

target_sources(
    oilc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
)
target_sources(
    oillib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internals/files/comp.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internals/files/decomp.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internals/files/files.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internals/misc/debug.cc
)
target_include_directories(
    oillib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/internals/files
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/internals/misc
)

target_link_libraries(
    oilc
    PRIVATE
        oillib
)
