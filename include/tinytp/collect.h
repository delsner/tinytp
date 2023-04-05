#ifndef TINYTP_COLLECT_H
#define TINYTP_COLLECT_H

#include <tinytp/runner.h>
#include <tinytp/report-parser.h>
#include <utility>
#include <memory>

namespace tinytp {
    class TinyTPCollector : public TinyTPRunner {
    public:
        explicit TinyTPCollector(const SQLiteDB &db, std::unique_ptr<ReportParser> &parser)
                : TinyTPRunner(db), parser(std::move(parser)) {}

        int run() override;

    private:
        std::unique_ptr<ReportParser> parser;
    };
}

#endif //TINYTP_COLLECT_H
