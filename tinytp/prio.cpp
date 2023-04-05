#include <iostream>
#include <sstream>
#include <algorithm>
#include <limits>

#include <tinytp/prio.h>
#include <tinytp/util.h>

namespace {
    /// Simple similarity function to compute min distance between test and each changed path.
    double maxPathSimilarity(const std::string &testName, std::vector<std::string> &changedPaths) {
        int minPathDistance = std::numeric_limits<int>::max();
        for (const auto &path: changedPaths) {
            auto distance = tinytp::string::levenshtein(testName, path);
            minPathDistance = std::min(minPathDistance, distance);
        }
        // TODO normalize?
        return minPathDistance == 0 ? 1.0 : 1.0 / minPathDistance;
    }
}

namespace tinytp {
    int TinyTPPrio::run() {
        std::vector<std::string> changedFiles;
        if (fs::exists(changesetFile)) {
            changedFiles = IO::readLinesFromFile(changesetFile);
        } else {
            std::cout << "warning: empty changeset file found." << std::endl;
        }
        // TODO parameterize which TP algo to use
        auto algo = HistoryPathDurationTP(granularity, db, changedFiles);
        auto prioritizedTests = algo.prioritize();
        if (prioritizedTests.empty()) {
            std::cout << "found no tests to prioritize" << std::endl;
            return 1;
        }
        std::stringstream output;
        for (const auto &test: prioritizedTests) {
            output << test.id << "," << test.score << "\n";
        }
        IO::writeFile(outputDir / "tinytp.prio", output.str());
        return 0;
    }

    std::vector<PrioritizedTest> HistoryPathDurationTP::prioritize() {
        db.connect();
        std::string sql;
        if (granularity == PrioGranularity::SUITE) {
            sql = "select test_suite_name as name, sum(failed), sum(total), avg(duration) from test_suite_execution;";
        } else {
            sql = "select test_module_name as name, sum(failed), sum(total), avg(duration) from test_suite_execution "
                  "group by test_module_name;";
        }
        auto row = db.select(sql);
        if (db.hasError()) {
            throw std::runtime_error("failed to query database for prioritization" + std::string(db.getError()));
        }
        std::vector<PrioritizedTest> tests;
        // TODO how to normalize?
        while (row.hasNext()) {
            auto name = row.get<std::string>();
            auto failed = static_cast<double>(row.get<int>());
            auto total = static_cast<double>(row.get<int>());
            auto duration = row.get<double>();
            double score = ((failed / total) * maxPathSimilarity(name, changedPaths)) / duration;
            tests.push_back(PrioritizedTest{name, score});
        }

        // order in descending order
        std::sort(tests.begin(), tests.end(),
                  [](const PrioritizedTest &t1, const PrioritizedTest &t2) { return t1.score > t2.score; });

        return tests;
    }

}

