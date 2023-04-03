#ifndef TINYTP_RUNNER_H
#define TINYTP_RUNNER_H

#include <filesystem>
#include <string>
#include <utility>

namespace tinytp {
    namespace fs = std::filesystem;

    /// Base interface for runners that can be configured using CLI parameters.
    class TinyTPRunner {
    public:
        explicit TinyTPRunner(std::string dbConnection) : dbConnection(std::move(dbConnection)) {};

        virtual ~TinyTPRunner() = default;

        virtual int run() = 0;

    protected:
        std::string dbConnection;
    };
}
#endif //TINYTP_RUNNER_H
