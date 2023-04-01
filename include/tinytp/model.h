#ifndef TINYTP_MODEL_H
#define TINYTP_MODEL_H

#include <string>
#include <optional>
#include <chrono>

namespace tinytp {

    using namespace std::chrono;

    /// Represents a test at arbitrary granularity level (e.g., test executable, test suite, test case).
    struct TestExecution {
        std::string testId;
        uint16_t failures = 0;
        uint64_t timestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        std::optional<std::string> path;
    };
}

#endif //TINYTP_MODEL_H
