
cmake_minimum_required(VERSION 3.20)

project(C_NMS LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_COVERAGE "Enable GCC code coverage" ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(nms_core
    src/device_manager.c
    src/monitoring.c
    src/fault_detection.c
    src/alarm_manager.c
    src/event_logger.c
    src/storage.c
)

target_include_directories(nms_core PUBLIC include)

add_executable(nms_gtest
    tests/test_nms.cpp
)

target_link_libraries(nms_gtest
    PRIVATE nms_core GTest::gtest_main
)

if(ENABLE_COVERAGE AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(nms_core PRIVATE
        --coverage -O0 -g
    )

    target_link_options(nms_core INTERFACE
        --coverage
    )

    target_compile_options(nms_gtest PRIVATE
        --coverage -O0 -g
    )

    target_link_options(nms_gtest PRIVATE
        --coverage
    )
endif()

enable_testing()

include(GoogleTest)
gtest_discover_tests(nms_gtest)
