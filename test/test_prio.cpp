#include <tinytp/prio.h>
#include <tinytp/model.h>
#include <gmock/gmock.h>
#include <filesystem>

using namespace testing;
using namespace tinytp;

namespace fs = std::filesystem;

class PrioTestSuite : public Test {
protected:
    void SetUp() override {
        db.connect();
        ASSERT_TRUE(db.execute(TestSuiteExecution::sqlCreateTable()));
        ASSERT_FALSE(db.hasError());
        ASSERT_FALSE(db.select("select * from test_suite_execution;").hasNext());
    }

    void TearDown() override {
        db.disconnect();
    }

    SQLiteDB db = SQLiteDB(":memory:");
};

TEST_F(PrioTestSuite, EmptyChangeset) {
    auto test1 = "foo.bar.Suite1";
    auto test2 = "foo.bar.Suite2";
    std::stringstream sql;
    sql << "INSERT INTO " << TestSuiteExecution::tableName << TestSuiteExecution::columns << " VALUES ";
    sql << "('" << test1 << "', 'foo.bar', 4, 8, 123, 1.0),";
    sql << "('" << test2 << "', 'foo.bar', 4, 4, 123, 0.5);";
    db.execute(sql.str());
    std::vector<std::string> changeset;
    auto algo = HistoryPathDurationTP(PrioGranularity::SUITE, db, changeset);
    auto actual = algo.prioritize();
    ASSERT_THAT(actual, ElementsAre(PrioritizedTest{test2}, PrioritizedTest{test1}));
}

TEST_F(PrioTestSuite, SingleElementChangeset) {
    auto test1 = "foo.bar.Suite1";
    auto test2 = "baz.Suite2";
    std::stringstream sql;
    sql << "INSERT INTO " << TestSuiteExecution::tableName << TestSuiteExecution::columns << " VALUES ";
    sql << "('" << test1 << "', 'foo.bar', 4, 8, 123, 1.0),";
    sql << "('" << test2 << "', 'baz', 4, 8, 123, 1.0);";
    db.execute(sql.str());
    std::vector<std::string> changeset{"baz/Suite2.cpp"};
    auto algo = HistoryPathDurationTP(PrioGranularity::SUITE, db, changeset);
    auto actual = algo.prioritize();
    ASSERT_THAT(actual, ElementsAre(PrioritizedTest{test2}, PrioritizedTest{test1}));
}

TEST_F(PrioTestSuite, ModuleLevelPrio) {
    auto module1 = "foo.bar";
    auto module2 = "foo.baz";
    std::stringstream sql;
    sql << "INSERT INTO " << TestSuiteExecution::tableName << TestSuiteExecution::columns << " VALUES ";
    sql << "('foo.bar.Suite1', '" << module1 << "', 0, 8, 123, 1.0),";
    sql << "('foo.bar.Suite2', '" << module1 << "', 0, 8, 123, 1.0),";
    sql << "('foo.baz.Suite1', '" << module2 << "', 8, 8, 123, 1.0);";
    db.execute(sql.str());
    std::vector<std::string> changeset{"baz/Suite2.cpp"};
    auto algo = HistoryPathDurationTP(PrioGranularity::MODULE, db, changeset);
    auto actual = algo.prioritize();
    ASSERT_THAT(actual, ElementsAre(PrioritizedTest{module2}, PrioritizedTest{module1}));
}
