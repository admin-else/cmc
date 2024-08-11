cmake_minimum_required(VERSION 3.26)
project(cmc_project C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_library(cmc
  src/type/c_types.c
  src/type/varnums.c
  src/type/pstring.c
  src/type/uuid.c
  src/buff.c
  src/err.c
  src/heap_utils.c
)

target_include_directories(cmc PUBLIC include)

if(NOT PROJECT_IS_TOP_LEVEL)
  return()
endif()

include(CTest)

add_executable(types_test tests/types.c)
target_compile_options(types_test PUBLIC -fsanitize=address,undefined)
target_link_options(types_test PUBLIC -fsanitize=address,undefined)
target_link_libraries(types_test PUBLIC cmc)
target_include_directories(types_test PUBLIC include)
add_test(NAME types_test COMMAND types_test)
