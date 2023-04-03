#include <tinytp/report-parser.h>
#include <tinytp/util.h>
#include <json/json.hpp>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

namespace tinytp {
    std::vector<TestSuiteExecution> JenkinsJsonReportParser::parse() {
        std::ifstream file(reportPath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string jsonString = buffer.str();

        auto root = json::parse(jsonString);
        std::vector<TestSuiteExecution> executions;
        if (root.contains("suites") && root["suites"].is_array()) {
            for (const auto &testSuite: root["suites"]) {
                std::string testSuiteName = testSuite["name"].get<std::string>();
                std::vector<std::string> testSuiteParts = string::split(testSuiteName, ".");
                testSuiteParts.pop_back();
                std::string testModuleName = string::join(testSuiteParts, ".");
                uint16_t total = 0;
                uint16_t failures = 0;
                double duration = testSuite["duration"].get<double>();

                if (testSuite.contains("cases") && testSuite["cases"].is_array() && !testSuite["cases"].empty()) {
                    for (const auto &testCase: testSuite["cases"]) {
                        ++total;
                        if (testCase["status"] == "FAILED" || testCase["status"] == "ERROR" ||
                            testCase["status"] == "REGRESSION") {
                            ++failures;
                        }
                    }
                }

                executions.push_back(TestSuiteExecution{
                        0,
                        testSuiteName,
                        testModuleName,
                        failures,
                        total,
                        static_cast<uint64_t>(duration_cast<milliseconds>(
                                system_clock::now().time_since_epoch()).count()),
                        duration
                });
            }
        }
        return executions;
    }
}
