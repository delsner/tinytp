#include <gtest/gtest.h>
#include <tinytp/cli.h>
#include <tinytp/model.h>
#include <tinytp/util.h>
#include <tinytp/collect.h>
#include <tinytp/prio.h>

using namespace ::testing;
using namespace tinytp;

namespace fs = std::filesystem;

class CliTestSuite : public Test {
protected:
    void SetUp() override {
        outputDir = fs::temp_directory_path();
        dbFile = outputDir / "tp.db";
        if (fs::exists(dbFile)) {
            fs::remove(dbFile);
        }
        SQLiteDB db(dbFile.string());
        db.connect();
        ASSERT_TRUE(db.execute(TestSuiteExecution::sqlCreateTable()));
        ASSERT_FALSE(db.hasError());
        db.disconnect();
        ASSERT_FALSE(db.isConnected());
        testReport = outputDir / "test-report.json";
        if (fs::exists(testReport)) {
            fs::remove(testReport);
        }
        IO::writeFile("{"
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
                      "}", testReport);
    }

    void TearDown() override {
        fs::remove(testReport);
        fs::remove(dbFile);
        fs::remove(outputDir / "tinytp.prio");
    }

    fs::path testReport;
    fs::path dbFile;
    fs::path outputDir;
};

TEST_F(CliTestSuite, ParseEndToEnd) {
    std::vector<std::string> argv;
    std::unique_ptr<TinyTPRunner> runner;
    // write into db
    argv = {{"tinytp"},
            {"collect"},
            {"--db"},
            {dbFile},
            {"--jenkins"},
            {testReport}};
    std::vector<std::string_view> args1{argv.begin() + 1, argv.end()};
    runner = parse(args1);
    ASSERT_NE(dynamic_cast<TinyTPCollector *>(runner.get()), nullptr);
    ASSERT_EQ(runner->run(), 0);

    // read from db
    argv = {{"tinytp"},
            {"prio"},
            {"--db"},
            {dbFile},
            {"--output"},
            {outputDir}};
    std::vector<std::string_view> args2{argv.begin() + 1, argv.end()};
    runner = parse(args2);
    ASSERT_NE(dynamic_cast<TinyTPPrio *>(runner.get()), nullptr);
    ASSERT_EQ(runner->run(), 0);
    ASSERT_TRUE(fs::exists(outputDir / "tinytp.prio"));
}
