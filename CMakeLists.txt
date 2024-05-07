cmake_minimum_required(VERSION 3.5)
project(cmc_project C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -Wall -Wextra -fPIC")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og -ggdb -Wall -Wextra -fPIC -fsanitize=address,undefined")

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include ${ZLIB_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${CURL_INCLUDE_DIR})

file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.c)

add_library(cmc SHARED ${SRC_FILES})
add_library(cmc_static STATIC ${SRC_FILES})

target_link_libraries(cmc PRIVATE ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES})
target_link_libraries(cmc_static PRIVATE ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES})

add_custom_target(format
    COMMAND clang-format -i ${SRC_FILES} ${CMAKE_SOURCE_DIR}/include/cmc/*
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(codegen
    COMMAND python3 codegen.py
    COMMAND clang-format -i ${SRC_FILES} ${CMAKE_SOURCE_DIR}/include/cmc/*
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

install(TARGETS cmc LIBRARY DESTINATION lib)
install(TARGETS cmc_static ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)

file(GLOB TEST_SOURCES ${CMAKE_SOURCE_DIR}/tests/*.c)
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE cmc ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()