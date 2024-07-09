cmake_minimum_required(VERSION 3.26)
project(cmc_project C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_library(cmc
  src/buffer.c
  src/error.c
)

target_include_directories(cmc PUBLIC include)

