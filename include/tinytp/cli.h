#ifndef TINYTP_CLI_H
#define TINYTP_CLI_H

#include <tinytp/runner.h>

namespace tinytp {

    std::unique_ptr<TinyTPRunner> parse(int argc, char *argv[]);

    std::unique_ptr<TinyTPRunner> parse(const std::vector<std::string_view> &args);

    void printHelpMessage(const char *reason);
}

#endif //TINYTP_CLI_H
