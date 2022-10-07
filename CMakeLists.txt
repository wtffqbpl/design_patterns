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

file(GLOB_RECURSE cpp_design_patterns_srcs *.cc)
add_executable(cpp_design_patterns
        chain_of_responsibility/chain_of_responsibility_test.cc
        command/command_test.cc
        composite/composite_test.cc
        decorator/decorator.cc
        facade/facade.cc
        flyweight/flyweight.cc
        interpreter/interpreter.cc
        iterator/iterator.cc
        mediator/mediator.cc
        memento/memento.cc
        observer/observer.cc
        prototype_pattern/prototype.cc
        singleton/singleton.cc
        state_pattern/state.cc
        strategy/strategy.cc
        template/template_pattern.cc
        visitor/visitor.cc
        )

include_directories(include
        chain_of_responsibility
        command
        composite
        decorator
        facade
        flyweight
        interpreter
        iterator
        mediator
        memento
        observer
        prototype_pattern
        singleton
        state_pattern
        strategy
        template
        visitor
        )

target_link_libraries(
        cpp_design_patterns
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cpp_design_patterns)
