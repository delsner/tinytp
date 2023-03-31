#include <tinytp/git.h>
#include <gtest/gtest.h>

using namespace tinytp::git;

TEST(GitTestSuite, AlwaysTrue) {
    ASSERT_EQ(diff(), true);
}