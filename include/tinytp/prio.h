#ifndef TINYTP_PRIO_H
#define TINYTP_PRIO_H

#include <tinytp/runner.h>
#include <tinytp/db.h>

#include <string>
#include <utility>
#include <vector>
#include <filesystem>
#include <utility>

namespace fs = std::filesystem;

namespace tinytp {

    enum class PrioGranularity {
        SUITE, MODULE
    };

    class TinyTPPrio : public TinyTPRunner {
    public:
        explicit TinyTPPrio(const SQLiteDB &db, fs::path outputDir, fs::path changesetFile,
                            const PrioGranularity granularity)
                : TinyTPRunner(db), outputDir(std::move(outputDir)), granularity(granularity),
                  changesetFile(std::move(changesetFile)) {}

        int run() override;

    private:
        fs::path changesetFile;
        fs::path outputDir;
        PrioGranularity granularity;
    };

    struct PrioritizedTest {
        std::string id;
        double score;
    };

    class BaseTP {
    public:
        explicit BaseTP(const PrioGranularity granularity, const SQLiteDB &db) : granularity(granularity), db(db) {}

        /// Returns prioritized list of test entities (test suite or module).
        virtual std::vector<PrioritizedTest> prioritize() = 0;

    protected:
        PrioGranularity granularity;
        SQLiteDB db;
    };

    /// This algorithm computes the prioritization using the formula:
    /// Score = (rel. failure freq. * max. path similarity) / duration
    class HistoryPathDurationTP : public BaseTP {
    public:
        HistoryPathDurationTP(const PrioGranularity granularity, const SQLiteDB &db,
                              std::vector<std::string> &changedPaths) : BaseTP(
                granularity, db), changedPaths(changedPaths) {}

        std::vector<PrioritizedTest> prioritize() override;

    private:
        std::vector<fs::path> changedPaths;
    };
}


#endif //TINYTP_PRIO_H
