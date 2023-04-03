#ifndef TINYTP_REPORT_PARSER_H
#define TINYTP_REPORT_PARSER_H

#include <filesystem>
#include <utility>
#include <vector>
#include <tinytp/model.h>

namespace tinytp {
    namespace fs = std::filesystem;

    class BaseReportParser {
    public:
        virtual std::vector<TestSuiteExecution> parse() = 0;
    };

    class JenkinsJsonReportParser : public BaseReportParser {
    public:
        explicit JenkinsJsonReportParser(fs::path reportPath) : reportPath(std::move(reportPath)) {}

        std::vector<TestSuiteExecution> parse() override;

    private:
        fs::path reportPath;
    };
}

#endif //TINYTP_REPORT_PARSER_H
