# Download GoogleTest source to build and link statically
include(FetchContent)
FetchContent_Declare(gtest
        QUIET
        URL https://github.com/google/googletest/archive/release-1.12.0.tar.gz
        )
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

# create binary for test execution
add_executable(tinytp_unittests main.cpp test_git.cpp)

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