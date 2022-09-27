cmake_minimum_required(VERSION 3.20)
project(intersim_test)
add_executable(intersim_test simple_test.cc)
target_link_libraries(intersim_test PRIVATE intersim)