add_library(tinytp_lib git.cpp)
target_include_directories(tinytp_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(tinytp main.cpp)
target_link_libraries(tinytp tinytp_lib)