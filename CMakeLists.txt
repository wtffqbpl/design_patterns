cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(cpp_design_patterns)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-std=c++2a")
enable_testing()

file(GLOB_RECURSE cpp_design_patterns_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
add_executable(cpp_design_patterns ${cpp_design_patterns_srcs})

target_link_libraries(
        cpp_design_patterns
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cpp_design_patterns)
