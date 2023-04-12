#ifndef TINYTP_REPORT_PARSER_H
#define TINYTP_REPORT_PARSER_H

#include <filesystem>
#include <utility>
#include <vector>
#include <tinytp/model.h>

namespace fs = std::filesystem;

namespace tinytp {

    class ReportParser {
    public:
        virtual ~ReportParser() = default;

        virtual std::vector<TestSuiteExecution> parse() = 0;
    };

    class JenkinsJsonReportParser : public ReportParser {
    public:
        explicit JenkinsJsonReportParser(fs::path reportPath) : reportPath(std::move(reportPath)) {}

        std::vector<TestSuiteExecution> parse() override;

    private:
        fs::path reportPath;
    };
}

#endif //TINYTP_REPORT_PARSER_H
