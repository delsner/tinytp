cmake_minimum_required(VERSION 3.18)

project(TinyTP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(tinytp)

enable_testing()
add_subdirectory(test)

