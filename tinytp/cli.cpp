#include <tinytp/cli.h>
#include <iostream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace tinytp {

    struct TinyTPPrio : public TinyTPRunner {
        TinyTPPrio(const std::filesystem::path &outputDir,
                   const std::filesystem::path &dbDir) : TinyTPRunner(outputDir,
                                                                      dbDir) {}

        int run() override {
            std::cout << "Prio" << std::endl;
            return 0;
        }
    };

    struct TinyTPCollector : public TinyTPRunner {
        TinyTPCollector(const std::filesystem::path &outputDir,
                        const std::filesystem::path &dbDir) : TinyTPRunner(
                outputDir, dbDir) {}

        int run() override {
            std::cout << "Collect" << std::endl;
            return 0;
        }
    };

    template<typename T>
    void checkFlag(
            const std::string &flag,
            const std::vector<const std::string_view> &args,
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
            const std::vector<const std::string_view> &args,
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

        const std::vector<const std::string_view> args(argv + 1, argv + argc);
        bool inCollectMode = false;
        fs::path dbDir = fs::current_path();
        fs::path outputDir = fs::current_path();
        fs::path gitDir = fs::current_path();
        bool useJUnit = false;

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
            checkFlag<fs::path>("--db", args, idx, dbDir);
            checkFlag<fs::path>("--output", args, idx, outputDir);
            checkFlag<fs::path>("--git", args, idx, gitDir);
            checkFlag<bool>("--junit", args, idx, useJUnit);
        }

        if (inCollectMode) {
            return std::make_unique<TinyTPCollector>(TinyTPCollector{outputDir, dbDir});
        }
        return std::make_unique<TinyTPPrio>(TinyTPPrio{outputDir, dbDir});
    }

    void printHelpMessage(const char *message) {
        std::cerr << "tinytp: " << message << "\n\n";
        std::cerr << "Usage: tinytp [OPTIONS] COMMAND\n\n";
        std::cerr << "A simple test prioritization tool.\n\n";
        std::cerr << "Options:\n";
        std::cerr << "\t--db string \t\tDirectory where to look for TinyTP database (default: current)\n";
        std::cerr << "\t--output string\t\tDirectory where to put any output (except database) (default: current)\n";
        std::cerr << "\t--git string\t\tRoot of git repository (default: current)\n";
        std::cerr << '\n';
        std::cerr << "Commands:\n";
        std::cerr << "\tcollect\t\tCollect data and store into TinyTP database\n";
        std::cerr << "\tprio\t\tPrioritize tests\n";
    }
}
