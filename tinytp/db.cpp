#include <sqlite3/sqlite3.h>
#include <tinytp/db.h>


namespace tinytp {
    bool SQLiteDB::connect() noexcept {
        if (db) {
            return true;
        }
        int rc = sqlite3_open(filename.c_str(), &db);
        if (rc) {
            errMsg = const_cast<char*>(sqlite3_errmsg(db));
            return false;
        }
        return true;
    }

    bool SQLiteDB::execute(const std::string &statement) {
        if (!isConnected()) {
            return false;
        }
        int rc = sqlite3_exec(db, statement.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            return false;
        }
        return true;
    }

    SQLiteDB::Row SQLiteDB::select(const std::string& sql) {
        if (!isConnected()) {
            return Row(nullptr);
        }
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            errMsg = const_cast<char*>(sqlite3_errmsg(db));
            return Row(nullptr);
        }
        return Row(stmt);
    }

    void tinytp::SQLiteDB::disconnect() noexcept {
        if (db) {
            sqlite3_close(db);
            db = nullptr;
            if (errMsg != nullptr) {
                sqlite3_free(errMsg);
            }
            errMsg = nullptr;
        }
    }
}
