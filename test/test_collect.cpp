#include <tinytp/collect.h>
#include <gtest/gtest.h>

using namespace testing;
using namespace tinytp;

TEST(DBTestSuite, Connect) {
    SQLiteDB db(filename);
    ASSERT_TRUE(db.isConnected());
    db.disconnect();
    ASSERT_FALSE(db.isConnected());
}

TEST(DBTestSuite, Execute) {
    SQLiteDB db(filename);
    ASSERT_TRUE(db.execute(createTableSql));
    ASSERT_FALSE(db.hasError());
    ASSERT_FALSE(db.select("SELECT * FROM PERSON;").hasNext());
}

TEST(DBTestSuite, ExecuteWithError) {
    SQLiteDB db(filename);
    ASSERT_FALSE(db.execute("CREATED TABLE PERSON();"));
    ASSERT_TRUE(db.hasError());
}

TEST(DBTestSuite, InsertAndQuery) {
    SQLiteDB db(filename);
    ASSERT_TRUE(db.execute(createTableSql));
    ASSERT_TRUE(db.execute(insertSql));
    ASSERT_TRUE(db.select("SELECT * FROM PERSON;").hasNext());
}

TEST(DBTestSuite, ConvertRowsToFields) {
    SQLiteDB db(filename);
    ASSERT_TRUE(db.execute(createTableSql));
    ASSERT_TRUE(db.execute(insertSql));
    auto row = db.select("SELECT * FROM PERSON;");
    ASSERT_FALSE(db.hasError());
    ASSERT_TRUE(row.hasNext()); // put cursor on first row
    auto id = row.get<int>();
    ASSERT_EQ(id, 1);
    auto name = row.get<std::string>();
    ASSERT_EQ(name, "Allen");
    auto age = row.get<int>();
    ASSERT_EQ(age, 25);
    auto address = row.get<std::string>();
    ASSERT_EQ(address, "Texas");
    auto salary = row.get<double>();
    ASSERT_EQ(salary, 1500.00);
    ASSERT_TRUE(row.hasNext()); // put cursor to next row
    id = row.get<int>();
    ASSERT_EQ(id, 2);
    name = row.get<std::string>();
    ASSERT_EQ(name, "Teddy");
    age = row.get<int>();
    ASSERT_EQ(age, 23);
    address = row.get<std::string>();
    ASSERT_EQ(address, "Norway");
    salary = row.get<double>();
    ASSERT_EQ(salary, 2000.00);
}