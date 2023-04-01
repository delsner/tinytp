#ifndef TINYTP_REPORT_PARSER_H
#define TINYTP_REPORT_PARSER_H

#include <filesystem>
#include <vector>
#include <tinytp/model.h>

namespace tinytp {
    namespace fs = std::filesystem;

    class BaseReportParser {
    public:
        virtual std::vector<TestExecution> parse() = 0;
    };

    class JUnitParser: public BaseReportParser {
    public:
        std::vector<TestExecution> parse() override;
    private:
        fs::path reportPath;
    };
}

#endif //TINYTP_REPORT_PARSER_H
