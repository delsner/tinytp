#include <tinytp/cli.h>
#include <tinytp/runner.h>

using namespace tinytp;

int main(int argc, char *argv[]) {
    std::unique_ptr<TinyTPRunner> runner;
    try {
        runner = parse(argc, argv);
    } catch (const std::exception &x) {
        printHelpMessage(x.what());
        return 1;
    }
    return runner->run();
}