cmake_minimum_required(VERSION 3.26)
project(cmc_project C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_library(cmc
  src/types/c_types.c
  src/buff.c
  src/err.c
  src/heap_utils.c
)

target_include_directories(cmc PUBLIC include)

if(NOT PROJECT_IS_TOP_LEVEL)
  return()
endif()

include(CTest)

add_executable(c_types_test tests/c_types.c)
target_link_libraries(c_types_test PUBLIC cmc)
target_include_directories(c_types_test PUBLIC include)
add_test(NAME c_types_test COMMAND c_types_test)
