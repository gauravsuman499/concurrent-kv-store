cmake_minimum_required(VERSION 4.0)

project(
    concurrent_kv_store
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(
    ENABLE_SANITIZERS
    "Enable AddressSanitizer and UndefinedBehaviorSanitizer"
    OFF
)

option(
    ENABLE_TSAN
    "Enable ThreadSanitizer"
    OFF
)

add_library(kv_store_warnings INTERFACE)

target_compile_options(kv_store_warnings INTERFACE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow
)

add_library(kv_store_lib
    src/kv_store.cpp
)

target_include_directories(kv_store_lib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(kv_store_lib
    PRIVATE
        kv_store_warnings
)

add_executable(kv_store
    src/main.cpp
)

target_link_libraries(kv_store
    PRIVATE
        kv_store_lib
        kv_store_warnings
)

enable_testing()

add_executable(kv_store_tests
    tests/kv_store_test.cpp
)

target_link_libraries(kv_store_tests
    PRIVATE
        kv_store_lib
        kv_store_warnings
)

add_test(
    NAME kv_store_tests
    COMMAND kv_store_tests
)

add_executable(kv_store_concurrent_tests
    tests/concurrent_kv_store_test.cpp
)

target_link_libraries(kv_store_concurrent_tests
    PRIVATE
        kv_store_lib
        kv_store_warnings
)

add_test(
    NAME kv_store_concurrent_tests
    COMMAND kv_store_concurrent_tests
)

add_executable(kv_store_benchmark
    benchmarks/kv_store_benchmark.cpp
)

target_link_libraries(kv_store_benchmark
    PRIVATE
        kv_store_lib
        kv_store_warnings
)

if(ENABLE_SANITIZERS)
    target_compile_options(kv_store_lib PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )

    target_link_options(kv_store_lib PRIVATE
        -fsanitize=address,undefined
    )

    target_compile_options(kv_store PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )

    target_link_options(kv_store PRIVATE
        -fsanitize=address,undefined
    )

    target_compile_options(kv_store_tests PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )

    target_link_options(kv_store_tests PRIVATE
        -fsanitize=address,undefined
    )

    target_compile_options(kv_store_concurrent_tests PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )

    target_link_options(kv_store_concurrent_tests PRIVATE
        -fsanitize=address,undefined
    )
endif()


if(ENABLE_TSAN)
    target_compile_options(kv_store_lib PRIVATE
        -fsanitize=thread
        -fno-omit-frame-pointer
    )

    target_link_options(kv_store_lib PRIVATE
        -fsanitize=thread
    )

    target_compile_options(kv_store PRIVATE
        -fsanitize=thread
        -fno-omit-frame-pointer
    )

    target_link_options(kv_store PRIVATE
        -fsanitize=thread
    )

    target_compile_options(kv_store_tests PRIVATE
        -fsanitize=thread
        -fno-omit-frame-pointer
    )

    target_link_options(kv_store_tests PRIVATE
        -fsanitize=thread
    )

    target_compile_options(kv_store_concurrent_tests PRIVATE
        -fsanitize=thread
        -fno-omit-frame-pointer
    )

    target_link_options(kv_store_concurrent_tests PRIVATE
        -fsanitize=thread
    )
endif()