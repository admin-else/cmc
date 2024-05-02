cmake_minimum_required(VERSION 3.5)
project(cmc_project C)

# Set Compiler Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -ggdb -Wall -Wextra -fPIC")
# Add necessary flags if needed
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined -DCMC_ERR_EXTRA")

# Find Libraries (if needed)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

# Include Directories
include_directories(include ${ZLIB_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${CURL_INCLUDE_DIR})

# Source Files
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.c)

# Create Executable/Library
add_library(cmc SHARED ${SRC_FILES})

# Link Libraries
target_link_libraries(cmc PRIVATE ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES})

# Custom Targets
add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/bin/cmc
    DEPENDS cmc
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(format
    COMMAND clang-format -i ${SRC_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(codegen
    COMMAND python3 codegen.py
    COMMAND clang-format -i ${SRC_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Clean Target
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin/*
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
