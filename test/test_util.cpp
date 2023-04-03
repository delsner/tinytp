#include <tinytp/util.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include <fstream>

using namespace testing;
using namespace tinytp;

namespace fs = std::filesystem;

class IOUtilTestSuite : public Test {
protected:
    void SetUp() override {
        tempFile = fs::temp_directory_path() / "test.txt";
        if (fs::exists(tempFile)) {
            fs::remove(tempFile);
        }
    }

    void TearDown() override {
        if (fs::exists(tempFile)) {
            fs::remove(tempFile);
        }
    }

    fs::path tempFile;
};

TEST_F(IOUtilTestSuite, ReadyEmptyFile) {
    std::ofstream ofs(tempFile);
    ofs.close();
    auto actual = IO::readFile(tempFile);
    ASSERT_TRUE(actual.empty());
}

TEST_F(IOUtilTestSuite, WriteAndRead) {
    IO::writeFile("foo", tempFile);
    ASSERT_TRUE(fs::exists(tempFile));
    auto actual = IO::readFile(tempFile);
    ASSERT_EQ(actual, "foo");
}

TEST(StringUtilTestSuite, Split) {
    auto actual = string::split("foo.bar.baz", ".");
    ASSERT_THAT(actual, ElementsAre("foo", "bar", "baz"));
}

TEST(StringUtilTestSuite, Join) {
    auto actual = string::join({"foo", "bar", "baz"}, ".");
    ASSERT_THAT(actual, "foo.bar.baz");
}