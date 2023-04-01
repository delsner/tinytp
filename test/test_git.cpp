#include <tinytp/git.h>
#include <gtest/gtest.h>

using namespace tinytp;

TEST(GitTestSuite, AlwaysTrue) {
    ASSERT_EQ(diff(), true);
}