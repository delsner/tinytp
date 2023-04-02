#include <gtest/gtest.h>
#include <tinytp/report-parser.h>
#include <filesystem>
#include <string>

using namespace tinytp;

namespace fs = std::filesystem;

namespace {

    fs::path writeToTemporaryFile(const std::string &content) {
        // TODO
        return "";
    }


    bool equalTestExecutions(const std::vector<TestSuiteExecution> &vec1, const std::vector<TestSuiteExecution> &vec2) {
        return true;
    }
}

TEST(JUnitReportParserTestSuite, ParseSingleSuite) {
    auto reportPath = writeToTemporaryFile(
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?> \n"
            "   <testsuites id=\"Run\" name=\"My Test\" tests=\"2\" failures=\"1\" time=\"0.010\">\n"
            "      <testsuite id=\"foo.bar.Suite1\" name=\"Suite1\" tests=\"2\" failures=\"1\" time=\"0.010\">\n"
            "         <testcase id=\"foo.bar.Suite1.Case1\" name=\"Case1\" time=\"0.08\">\n"
            "            <failure message=\"...\" type=\"WARNING\" />\n"
            "         </testcase>\n"
            "         <testcase id=\"foo.bar.Suite1.Case2\" name=\"Case2\" time=\"0.02\" />\n"
            "       </testsuite>\n"
            "   </testsuites>");
    auto parser = JUnitParser(reportPath);
    auto expected = std::vector<TestSuiteExecution>{
            TestSuiteExecution{
                    0,
                    "foo.bar.Suite1",
                    "foo.bar",
                    1,
                    2,
                    123,
                    0.08
            }
    };

    auto actual = parser.parse();
    ASSERT_TRUE(equalTestExecutions(actual, expected));
}