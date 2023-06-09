include(../cmake/GoogleTest.cmake)

# create binary for test execution
add_executable(tinytp_unittests
        main.cpp
        test_db.cpp
        test_report_parser.cpp
        test_util.cpp
        test_collect.cpp
        test_prio.cpp
        test_cli.cpp)

# link gtest as static library
target_link_libraries(tinytp_unittests
        gtest_main
        gmock_main
        tinytp_lib
        )

# automatic discovery of unit tests
include(GoogleTest)
gtest_discover_tests(tinytp_unittests
        PROPERTIES
        LABELS "unit"
        DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
        240
        )
