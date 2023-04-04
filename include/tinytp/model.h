#ifndef TINYTP_MODEL_H
#define TINYTP_MODEL_H

#include <string>
#include <chrono>

namespace tinytp {

    using namespace std::chrono;

    struct TestSuiteExecution {
        uint64_t id;
        std::string testSuiteName;
        std::string testModuleName;
        uint16_t failedCases = 0;
        uint16_t totalCases = 0;
        uint64_t timestamp = static_cast<uint64_t>(duration_cast<milliseconds>(
                system_clock::now().time_since_epoch()).count());
        double duration;

        static constexpr const char *tableName = "TEST_SUITE_EXECUTION";
        static constexpr const char *columns = "(TEST_SUITE_NAME,TEST_MODULE_NAME,FAILED,TOTAL,TIMESTAMP,DURATION)";

        static constexpr const char *sqlCreateTable() {
            return "CREATE TABLE IF NOT EXISTS TEST_SUITE_EXECUTION ("
                   "ID                  INTEGER PRIMARY KEY,"
                   "TEST_SUITE_NAME     TEXT                NOT NULL,"
                   "TEST_MODULE_NAME    TEXT                NOT NULL,"
                   "FAILED              INT                 NOT NULL,"
                   "TOTAL               INT                 NOT NULL,"
                   "TIMESTAMP           INT                 NOT NULL,"
                   "DURATION            REAL                NOT NULL);";
        }

        bool operator==(const TestSuiteExecution &rhs) const {
            return id == rhs.id &&
                   testSuiteName == rhs.testSuiteName;
        }

        bool operator!=(const TestSuiteExecution &rhs) const {
            return !(rhs == *this);
        }
    };

}

#endif //TINYTP_MODEL_H
