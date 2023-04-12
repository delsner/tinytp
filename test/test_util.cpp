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

TEST_F(IOUtilTestSuite, ReadLines) {
    IO::writeFile("foo\nbar\nbaz", tempFile);
    ASSERT_TRUE(fs::exists(tempFile));
    auto actual = IO::readLinesFromFile(tempFile);
    ASSERT_THAT(actual, ElementsAre("foo", "bar", "baz"));
}

TEST(StringUtilTestSuite, Split) {
    auto actual = string::split("foo.bar.baz", ".");
    ASSERT_THAT(actual, ElementsAre("foo", "bar", "baz"));
}

TEST(StringUtilTestSuite, Join) {
    auto actual = string::join({"foo", "bar", "baz"}, ".");
    ASSERT_THAT(actual, "foo.bar.baz");
}

TEST(StringUtilTestSuite, LevenshteinDistance) {
    std::string str1 = "hello";
    std::string str2 = "world";
    int distance = string::levenshtein(str1, str2);
    EXPECT_EQ(distance, 4);
}

TEST(StringUtilTestSuite, LevenshteinSameStrings) {
    std::string str1 = "hello";
    std::string str2 = "hello";
    int distance = string::levenshtein(str1, str2);
    EXPECT_EQ(distance, 0);
}

TEST(StringUtilTestSuite, LevenshteinEmptyStrings) {
    std::string str1;
    std::string str2;
    int distance = string::levenshtein(str1, str2);
    EXPECT_EQ(distance, 0);
}

TEST(StringUtilTestSuite, LevenshteinOneEmptyString) {
    std::string str1 = "hello";
    std::string str2;
    int distance = string::levenshtein(str1, str2);
    EXPECT_EQ(distance, 5);
}

TEST(StringUtilTestSuite, LevenshteinLongStrings) {
    std::string str1 = "The quick brown fox jumps over the lazy dog";
    std::string str2 = "The quick brown dog jumps over the lazy fox";
    int distance = string::levenshtein(str1, str2);
    EXPECT_EQ(distance, 4);
}