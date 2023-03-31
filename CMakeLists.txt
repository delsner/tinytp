cmake_minimum_required(VERSION 3.18)

project(TinyTP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

add_subdirectory(tinytp)
add_subdirectory(external)

enable_testing()
add_subdirectory(test)

