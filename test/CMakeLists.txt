# Download GoogleTest source to build and link statically
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# create binary for test execution
add_executable(tinytp_unittests main.cpp test_git.cpp)

# link gtest as static library
target_link_libraries(tinytp_unittests
        gtest_main
        gmock_main
        tinytp_lib
        )