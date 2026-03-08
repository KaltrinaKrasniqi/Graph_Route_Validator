cmake_minimum_required(VERSION 3.16)

project(validator_core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    include
    third_party
)

file(GLOB_RECURSE SOURCES
    src/*.cc
    src/*.cpp
    src/*/*.cc
    src/*/*.cpp
)

add_executable(validator
    main.cc
    ${SOURCES}
)