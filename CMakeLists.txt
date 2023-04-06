cmake_minimum_required(VERSION 3.18)

project(TinyTP)

set(CMAKE_CXX_STANDARD 17)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif(UNIX AND NOT APPLE)

add_subdirectory(external)

# Add CXX coverage compiler flags
if (CODE_COVERAGE AND UNIX)
    include(cmake/CodeCoverage.cmake)
    append_coverage_compiler_flags()
endif (CODE_COVERAGE AND UNIX)

add_subdirectory(tinytp)

enable_testing()
add_subdirectory(test)

