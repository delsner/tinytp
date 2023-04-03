#ifndef TINYTP_COLLECT_H
#define TINYTP_COLLECT_H

#include <tinytp/runner.h>
#include <tinytp/report-parser.h>
#include <utility>

namespace tinytp {
    class TinyTPCollector : public TinyTPRunner {
    public:
        explicit TinyTPCollector(const std::string &dbConnection, fs::path jenkinsReport)
                : TinyTPRunner(dbConnection), jenkinsReport(std::move(jenkinsReport)) {}

        int run() override;

    private:
        fs::path jenkinsReport;
    };
}

#endif //TINYTP_COLLECT_H
