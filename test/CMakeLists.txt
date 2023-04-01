include(../cmake/googletest.cmake)

# create binary for test execution
add_executable(tinytp_unittests main.cpp test_git.cpp test_db.cpp)

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