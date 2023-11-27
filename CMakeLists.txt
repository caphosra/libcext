cmake_minimum_required(VERSION 3.20)

project(libmodc VERSION 1.0.0)

set(LIBMODC_TEST OFF CACHE BOOL "Emit the executable file to test this library.")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Required to use clangd.")

file(GLOB LIBMODC_SOURCE ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB LIBMODC_TEST_SOURCE ${PROJECT_SOURCE_DIR}/test/*.c)

set(LIBMODC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${LIBMODC_INCLUDE_DIR})

if(LIBMODC_TEST)
    enable_testing()

    add_executable(libmodc_test ${LIBMODC_SOURCE} ${LIBMODC_TEST_SOURCE})
    target_include_directories(libmodc_test PRIVATE ${PROJECT_SOURCE_DIR}/test/include)

    macro(add_modc_test)
        add_test(NAME ${ARGV0} COMMAND $<TARGET_FILE:libmodc_test> ${ARGV0})
        set_property(TEST ${ARGV0} PROPERTY LABELS ${ARGV1})
    endmacro()

    add_modc_test(test_result_ok_err test_result)
    add_modc_test(test_result_unwrap test_result)
endif()
