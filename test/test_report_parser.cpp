#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include <string>

#include <tinytp/util.h>
#include <tinytp/report-parser.h>

using namespace testing;
using namespace tinytp;

namespace fs = std::filesystem;

class JUnitReportParserTestSuite : public Test {
protected:
    void SetUp() override {
        tempFile = fs::temp_directory_path() / "test-report.json";
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

TEST_F(JUnitReportParserTestSuite, ParseSingleSuite) {
    auto reportPath = IO::writeFile("{"
                                    "    \"suites\": ["
                                    "        {"
                                    "            \"duration\": 0.08,"
                                    "            \"name\": \"foo.bar.Suite1\","
                                    "            \"cases\": ["
                                    "                    {"
                                    "                        \"name\": \"Case1\","
                                    "                        \"status\": \"FAILED\","
                                    "                        \"duration\": 0.01"
                                    "                    },"
                                    "                    {"
                                    "                        \"name\": \"Case2\","
                                    "                        \"status\": \"PASSED\","
                                    "                        \"duration\": 0.07"
                                    "                    }"
                                    "            ]"
                                    "        }"
                                    "    ]"
                                    "}", tempFile);
    auto parser = JenkinsJsonReportParser(reportPath);
    auto actual = parser.parse();
    ASSERT_THAT(actual, ElementsAre(TestSuiteExecution{
            0,
            "foo.bar.Suite1",
            "foo.bar",
            1,
            2,
            123,
            0.08
    }));
}