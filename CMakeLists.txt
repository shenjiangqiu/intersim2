cmake_minimum_required(VERSION 3.20)
project(intersim)
aux_source_directory(./src SRC_LIST)
aux_source_directory(./src/routers SRC_LIST_ROUTER)
aux_source_directory(./src/power SRC_LIST_POWER)
aux_source_directory(./src/networks SRC_LIST_networks)
aux_source_directory(./src/arbiters SRC_LIST_arbiters)
aux_source_directory(./src/allocators SRC_LIST_allocators)
set(CMAKE_CXX_STANDARD 17)

add_library(intersim ${SRC_LIST} ${SRC_LIST_ROUTER} ${SRC_LIST_POWER} ${SRC_LIST_networks} ${SRC_LIST_arbiters} ${SRC_LIST_allocators})
target_include_directories(intersim PUBLIC  src src/routers  src/networks  src/arbiters  src/allocators  src/power)

add_subdirectory(tests)