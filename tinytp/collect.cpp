#include <tinytp/collect.h>

namespace tinytp {
    int TinyTPCollector::run() {
        auto parser = JUnitParser(junitReport);
        auto executions = parser.parse();

        return 0;
    }
}