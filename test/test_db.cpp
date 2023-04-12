#include <tinytp/db.h>
#include <gtest/gtest.h>

using namespace tinytp;

namespace {
    const char *filename = ":memory:";
    const char *createTableSql = "CREATE TABLE PERSON("
                                 "ID INTEGER PRIMARY KEY,"
                                 "NAME           TEXT    NOT NULL,"
                                 "AGE            INTEGER     NOT NULL,"
                                 "ADDRESS        CHAR(50),"
                                 "SALARY         REAL );";
    const char *insertSql = "INSERT INTO PERSON "
                            "(NAME,AGE,ADDRESS,SALARY) "
                            "VALUES "
                            "('Allen', 25, 'Texas', 1500.00),"
                            "('Teddy', 23, 'Norway', 2000.00);";
}

TEST(DBTestSuite, Connect) {
    SQLiteDB db(filename);
    ASSERT_FALSE(db.isConnected());
    db.connect();
    ASSERT_TRUE(db.isConnected());
    db.disconnect();
    ASSERT_FALSE(db.isConnected());
}

TEST(DBTestSuite, Execute) {
    SQLiteDB db(filename);
    db.connect();
    ASSERT_TRUE(db.execute(createTableSql));
    ASSERT_FALSE(db.hasError());
    ASSERT_FALSE(db.select("SELECT * FROM PERSON;").hasNext());
}

TEST(DBTestSuite, ExecuteWithError) {
    SQLiteDB db(filename);
    db.connect();
    ASSERT_FALSE(db.execute("CREATED TABLE PERSON();"));
    ASSERT_TRUE(db.hasError());
}

TEST(DBTestSuite, InsertAndQuery) {
    SQLiteDB db(filename);
    db.connect();
    ASSERT_TRUE(db.execute(createTableSql));
    ASSERT_TRUE(db.execute(insertSql));
    ASSERT_TRUE(db.select("SELECT * FROM PERSON;").hasNext());
}

TEST(DBTestSuite, ConvertRowsToFields) {
    SQLiteDB db(filename);
    db.connect();
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