cmake_minimum_required(VERSION 4.0)

project(
    concurrent_kv_store
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(kv_store
    src/main.cpp
)

target_compile_options(kv_store PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
