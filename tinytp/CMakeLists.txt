add_library(tinytp_lib
        db.cpp
        cli.cpp
        report-parser.cpp
        prio.cpp
        collect.cpp
        util.cpp)
target_include_directories(tinytp_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_include_directories(tinytp_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/include)
target_link_libraries(tinytp_lib
        sqlite3_static)

add_executable(tinytp main.cpp)
target_link_libraries(tinytp tinytp_lib)