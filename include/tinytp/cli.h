#ifndef TINYTP_CLI_H
#define TINYTP_CLI_H

#include <memory>
#include <filesystem>

namespace tinytp {

    class TinyTPRunner {
    public:
        TinyTPRunner(std::filesystem::path outputDir, std::filesystem::path dbDir) : outputDir(std::move(
                outputDir)), dbDir(std::move(dbDir)) {};

        virtual ~TinyTPRunner() = default;

        virtual int run() = 0;

    protected:
        std::filesystem::path outputDir;
        std::filesystem::path dbDir;
    };

    std::unique_ptr<TinyTPRunner> parse(int argc, char *argv[]);

    void printHelpMessage(const char *reason);
}

#endif //TINYTP_CLI_H
