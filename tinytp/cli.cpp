#include <tinytp/cli.h>
#include <tinytp/runner.h>
#include <tinytp/collect.h>
#include <tinytp/prio.h>

#include <iostream>
#include <utility>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace tinytp {

    template<typename T>
    void checkFlag(
            const std::string &flag,
            const std::vector<std::string_view> &args,
            size_t &idx,
            T &dst) {
        const auto arg = args[idx];
        if (arg == flag) {
            if (args.size() - 1 != idx)
                dst = args[++idx];
        }
    };

    template<>
    void checkFlag(
            const std::string &flag,
            const std::vector<std::string_view> &args,
            size_t &idx,
            bool &dst) {
        const auto arg = args[idx];
        if (arg == flag) {
            dst = true;
        }
    };

    std::unique_ptr<TinyTPRunner> parse(int argc, char *argv[]) {
        if (argc < 2 || argc > 64) {
            throw std::runtime_error("invalid number of command line arguments");
        }

        const std::vector<std::string_view> args(argv + 1, argv + argc);
        bool inCollectMode = false;
        std::string dbConnection = fs::current_path() / "tinytp.db";
        fs::path outputDir = fs::current_path();
        fs::path changesetFile = fs::current_path() / "changeset.txt";
        fs::path junitReportDir = fs::current_path();
        bool printPrio = false;

        for (size_t idx = 0; idx < args.size(); ++idx) {
            const auto arg = args[idx];
            if (idx == 0) {
                if (arg == "collect") {
                    inCollectMode = true;
                    continue;
                } else if (arg == "prio") {
                    inCollectMode = false;
                    continue;
                }
                throw std::runtime_error("invalid mode: " + std::string(arg));
            }
            checkFlag<std::string>("--db", args, idx, dbConnection);
            checkFlag<fs::path>("--output", args, idx, outputDir);
            checkFlag<fs::path>("--changes", args, idx, changesetFile);
            checkFlag<fs::path>("--junit", args, idx, junitReportDir);
            checkFlag<bool>("--print", args, idx, printPrio);
        }

        if (inCollectMode) {
            return std::make_unique<TinyTPCollector>(TinyTPCollector{dbConnection, junitReportDir});
        }
        return std::make_unique<TinyTPPrio>(TinyTPPrio{dbConnection, outputDir, changesetFile});
    }

    void printHelpMessage(const char *message) {
        std::cerr << "tinytp: " << message << "\n\n";
        std::cerr << "Usage: tinytp [OPTIONS] COMMAND\n\n";
        std::cerr << "A simple test prioritization tool.\n\n";
        std::cerr << "Options:\n";
        std::cerr << "\t--db string \t\tPath to TinyTP database (default: tinytp.db)\n";
        std::cerr << "\t--output string\t\tDirectory where to put any output (except database) (default: current)\n";
        std::cerr << "\t--changes string\t\tPath to file containing files in changeset (default: changeset.txt)\n";
        std::cerr << "\t--junit string\t\tDirectory where to look for junit XML reports (default: current)\n";
        std::cerr << "\t--print string\t\tPrint prioritized tests to standard output\n";
        std::cerr << '\n';
        std::cerr << "Commands:\n";
        std::cerr << "\tcollect\t\tCollect data and store into TinyTP database\n";
        std::cerr << "\tprio\t\tPrioritize tests\n";
    }
}
