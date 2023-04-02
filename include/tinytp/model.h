#ifndef TINYTP_MODEL_H
#define TINYTP_MODEL_H

#include <string>
#include <optional>
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

        static constexpr const char * sqlCreateTable() {
            return "create table ()";
        }
    };

}

#endif //TINYTP_MODEL_H
