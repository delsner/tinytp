#include <tinytp/db.h>
#include <gtest/gtest.h>

using namespace tinytp;

TEST(DBTestSuite, AlwaysTrue) {
    ASSERT_EQ(connect(), true);
}