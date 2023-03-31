cmake_minimum_required(VERSION 3.18)

project(TinyTP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(test)

enable_testing()

add_subdirectory(tinytp)
