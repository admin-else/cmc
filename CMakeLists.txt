cmake_minimum_required(VERSION 3.14)
project(cmc_project C)

# Use cmake generator expressions https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -Wall -Wextra -Werror=switch-enum")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og -ggdb -Wall -Wextra -Werror=switch-enum -fsanitize=address,undefined")

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.c)

add_library(cmc ${SRC_FILES})
target_link_libraries(cmc PRIVATE ZLIB::ZLIB OpenSSL::SSL CURL::libcurl)
target_include_directories(cmc PUBLIC include)

add_custom_target(format
    COMMAND clang-format -i src/* include/cmc/*
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(codegen
    COMMAND python3 codegen.py
    COMMAND clang-format -i src/* include/cmc/*
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

if(NOT PROJECT_IS_TOP_LEVEL)
    return()
endif()

include(CTest)

if(BUILD_TESTING)
    file(GLOB TEST_SOURCES tests/*.c)
    foreach(TEST_SOURCE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
        set(TEST_TARGET "${TEST_NAME}_test")
        add_executable(${TEST_TARGET} ${TEST_SOURCE})
        target_link_libraries(${TEST_TARGET} PRIVATE cmc)
    endforeach()

    add_test(NAME status COMMAND status_test)
endif()

include(GNUInstallDirs)

install(TARGETS cmc)
install(DIRECTORY include/ TYPE INCLUDE)
