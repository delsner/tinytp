#ifndef TINYTP_COLLECT_H
#define TINYTP_COLLECT_H

#include <tinytp/runner.h>
#include <tinytp/report-parser.h>

namespace tinytp {
    class TinyTPCollector : public TinyTPRunner {
    public:
        explicit TinyTPCollector(const std::string &dbConnection, fs::path junitReport)
                : TinyTPRunner(dbConnection), junitReport(std::move(junitReport)) {}

        int run() override;

    private:
        fs::path junitReport;
    };
}

#endif //TINYTP_COLLECT_H
