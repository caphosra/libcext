cmake_minimum_required(VERSION 3.20)

project(libcpoly VERSION 1.0.0)

set(LIBCPOLY_TEST OFF CACHE BOOL "Emit the executable file to test this library.")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Required to use clangd.")

file(GLOB LIBCPOLY_SOURCE ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB LIBCPOLY_TEST_SOURCE ${PROJECT_SOURCE_DIR}/test/*.c)

set(LIBCPOLY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${LIBCPOLY_INCLUDE_DIR})

if(LIBCPOLY_TEST)
    enable_testing()

    add_executable(libcpoly_test ${LIBCPOLY_SOURCE} ${LIBCPOLY_TEST_SOURCE})
    target_include_directories(libcpoly_test PRIVATE ${PROJECT_SOURCE_DIR}/test/include)

    macro(add_cpoly_test)
        add_test(NAME ${ARGV0} COMMAND $<TARGET_FILE:libcpoly_test> ${ARGV0})
        set_property(TEST ${ARGV0} PROPERTY LABELS ${ARGV1})
    endmacro()

    add_cpoly_test(test_option_some_none test_option)
    add_cpoly_test(test_option_unwrap_op test_option)

    add_cpoly_test(test_result_ok_err test_result)
    add_cpoly_test(test_result_unwrap test_result)
endif()
