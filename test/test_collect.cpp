#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <utility>
#include <tinytp/collect.h>
#include <tinytp/report-parser.h>
#include <tinytp/db.h>

using namespace ::testing;
using namespace tinytp;

class MockParser : public ReportParser {
public:
    MOCK_METHOD(std::vector<TestSuiteExecution>, parse, (), (override));
};

TEST(CollectorTestSuite, SimpleAdd) {
    std::unique_ptr<MockParser> parser = std::make_unique<MockParser>();
    EXPECT_CALL(*parser, parse())
            .Times(Exactly(1))
            .WillRepeatedly(Return(std::vector<TestSuiteExecution>{
                    TestSuiteExecution{
                            1,
                            "foo.bar.FooSuite",
                            "foo.bar",
                            3,
                            5,
                            123,
                            0.1
                    },
                    TestSuiteExecution{
                            2,
                            "foo.bar.BarSuite",
                            "foo.bar",
                            0,
                            4,
                            123,
                            0.5
                    }
            }));
    // Convert to base parser type
    std::unique_ptr<ReportParser> parserBase = std::unique_ptr<MockParser>(parser.release());
    auto collector = TinyTPCollector(SQLiteDB(":memory:"), parserBase);
    ASSERT_EQ(collector.run(), 0);
}