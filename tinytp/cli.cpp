#include <tinytp/cli.h>
#include <tinytp/runner.h>
#include <tinytp/collect.h>
#include <tinytp/prio.h>
#include <tinytp/db.h>
#include <tinytp/report-parser.h>

#include <iostream>
#include <utility>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace tinytp {

    template<typename T>
    void parseFlag(
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
    void parseFlag(
            const std::string &flag,
            const std::vector<std::string_view> &args,
            size_t &idx,
            bool &dst) {
        const auto arg = args[idx];
        if (arg == flag) {
            dst = true;
        }
    };

    std::unique_ptr<TinyTPRunner> parse(const std::vector<std::string_view>& args) {
        bool inCollectMode = false;
        fs::path dbFile = fs::current_path() / "tinytp.db";
        fs::path outputDir = fs::current_path();
        fs::path changesetFile = fs::current_path() / "changeset.txt";
        fs::path jenkinsReport = fs::current_path() / "test-report.json";
        bool modulePrio = false;
        bool showHelp = false;

        for (size_t idx = 0; idx < args.size(); ++idx) {
            const auto arg = args[idx];
            if (idx == 0) {
                if (arg == "collect") {
                    inCollectMode = true;
                    continue;
                } else if (arg == "prio") {
                    inCollectMode = false;
                    continue;
                } else if (arg == "--help") {
                    showHelp = true;
                    continue;
                }
                throw std::runtime_error("invalid mode: " + std::string(arg));
            }
            parseFlag<fs::path>("--db", args, idx, dbFile);
            parseFlag<fs::path>("--output", args, idx, outputDir);
            parseFlag<fs::path>("--changes", args, idx, changesetFile);
            parseFlag<fs::path>("--jenkins", args, idx, jenkinsReport);
            parseFlag<bool>("--module", args, idx, modulePrio);
            parseFlag<bool>("--help", args, idx, showHelp);
        }

        if (showHelp) {
            throw std::runtime_error("help");
        }

        if (!inCollectMode && !fs::exists(dbFile)) {
            throw std::runtime_error("invalid database file provided at " + dbFile.string());
        }

        SQLiteDB db(dbFile.string());

        if (inCollectMode) {
            std::unique_ptr<ReportParser> parser;
            if (!jenkinsReport.empty() && fs::exists(jenkinsReport)) {
                parser = std::make_unique<JenkinsJsonReportParser>(jenkinsReport);
            } else {
                throw std::runtime_error("invalid test report provided to collect from");
            }
            return std::make_unique<TinyTPCollector>(TinyTPCollector{db, parser});
        }

        return std::make_unique<TinyTPPrio>(TinyTPPrio{db, outputDir, changesetFile,
                                                       modulePrio ? PrioGranularity::MODULE : PrioGranularity::SUITE});
    }

    std::unique_ptr<TinyTPRunner> parse(int argc, char *argv[]) {
        if (argc < 2 || argc > 64) {
            throw std::runtime_error("invalid number of command line arguments");
        }
        const std::vector<std::string_view> args(argv + 1, argv + argc);
        return parse(args);
    }

    void printHelpMessage(const char *message) {
        std::cerr << "tinytp: " << message << "\n\n";
        std::cerr << "Usage: tinytp [OPTIONS] COMMAND\n\n";
        std::cerr << "A simple test prioritization tool.\n\n";
        std::cerr << "Options:\n";
        std::cerr << "\t--db string \t\tPath to TinyTP database (default: tinytp.db)\n";
        std::cerr << "\t--output string\t\tDirectory where to put any output (except database) (default: current)\n";
        std::cerr << "\t--changes string\tPath to file containing files in changeset (default: changeset.txt)\n";
        std::cerr << "\t--jenkins string\tPath to Jenkins JSON report (default: test-report.json)\n";
        std::cerr << "\t--module\t\tPrioritize at test module level\n";
        std::cerr << '\n';
        std::cerr << "Commands:\n";
        std::cerr << "\tcollect\t\tCollect data and store into TinyTP database\n";
        std::cerr << "\tprio\t\tPrioritize tests\n";
    }
}
