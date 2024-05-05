cmake_minimum_required(VERSION 3.5)
project(cmc_project C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -Wall -Wextra -fPIC")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -ggdb -Wall -Wextra -fPIC -fsanitize=address,undefined -DCMC_ERR_EXTRA")

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/inclucde ${ZLIB_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${CURL_INCLUDE_DIR})

file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.c)

add_library(cmc SHARED ${SRC_FILES})
add_library(cmc_static STATIC ${SRC_FILES})

target_link_libraries(cmc PRIVATE ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES})
target_link_libraries(cmc_static PRIVATE ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES})

add_custom_target(format
    COMMAND clang-format -i ${SRC_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(codegen
    COMMAND python3 codegen.py
    COMMAND clang-format -i ${SRC_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

install(TARGETS cmc LIBRARY DESTINATION lib)
install(TARGETS cmc_static ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)
