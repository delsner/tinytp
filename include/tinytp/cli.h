#ifndef TINYTP_CLI_H
#define TINYTP_CLI_H

#include <tinytp/runner.h>

namespace tinytp {

    std::unique_ptr<TinyTPRunner> parse(int argc, char *argv[]);

    void printHelpMessage(const char *reason);
}

#endif //TINYTP_CLI_H
