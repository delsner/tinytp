#include <iostream>
#include <sstream>

#include <tinytp/collect.h>
#include <tinytp/db.h>
#include <tinytp/model.h>

namespace tinytp {
    int TinyTPCollector::run() {
        auto parser = JenkinsJsonReportParser(jenkinsReport);
        auto testSuiteExecutions = parser.parse();
        if (!testSuiteExecutions.empty()) {
            SQLiteDB db(dbConnection);
            db.execute(TestSuiteExecution::sqlCreateTable());
            if (db.hasError()) {
                std::cerr << "Unable to initialize database: " << db.getError() << std::endl;
                return 1;
            }
            std::stringstream sql;
            sql << "INSERT INTO " << TestSuiteExecution::tableName << TestSuiteExecution::columns << " VALUES ";
            for (size_t i = 0; i < testSuiteExecutions.size(); ++i) {
                auto testSuite = testSuiteExecutions[i];
                sql << "(,"
                    << "'" << testSuite.testSuiteName << "'" << ","
                    << "'" << testSuite.testModuleName << "'" << ","
                    << testSuite.failedCases << ","
                    << testSuite.totalCases << ","
                    << testSuite.timestamp << ","
                    << testSuite.duration << ")";
                if (i < testSuiteExecutions.size() - 1) {
                    sql << ",";
                }
            }
            sql << ";";
            db.execute(sql.str());
            if (db.hasError()) {
                std::cerr << "Unable to write to database: " << db.getError() << std::endl;
                return 1;
            }
        }
        return 0;
    }
}