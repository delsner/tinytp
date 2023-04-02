#ifndef TINYTP_PRIO_H
#define TINYTP_PRIO_H

#include <tinytp/runner.h>

#include <string>
#include <vector>
#include <filesystem>

namespace tinytp {

    namespace fs = std::filesystem;

    class TinyTPPrio : public TinyTPRunner {
    public:
        explicit TinyTPPrio(const std::string &dbConnection, fs::path outputDir, fs::path changesetFile)
                : TinyTPRunner(dbConnection), outputDir(std::move(outputDir)),
                  changesetFile(std::move(changesetFile)) {}

        int run() override;

    private:
        fs::path changesetFile;
        fs::path outputDir;
    };

    enum class PrioGranularity {
        SUITE, MODULE
    };

    class BaseTP {
    public:
        explicit BaseTP(PrioGranularity granularity) : granularity(granularity) {}

        /// Returns prioritized list of test entities (test suite or module).
        virtual std::vector<std::string> prioritize() = 0;

    protected:
        PrioGranularity granularity;
        std::string dbConnection;
    };

    /// This algorithm computes the prioritization using the formula:
    /// Score = (rel. failure freq. * max. path similarity) / duration
    class HistoryPathDurationTP : public BaseTP {
    public:
        HistoryPathDurationTP(PrioGranularity granularity, const std::vector<std::string> &changedPaths) : BaseTP(
                granularity), changedPaths(changedPaths) {}

        std::vector<std::string> prioritize() override;

    private:
        const std::vector<std::string> changedPaths;
    };
}


#endif //TINYTP_PRIO_H
