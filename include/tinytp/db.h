#ifndef TINYTP_DB_H
#define TINYTP_DB_H

#include <sqlite3/sqlite3.h>
#include <string>
#include <utility>
#include <iostream>

namespace tinytp {
    class SQLiteDB {
    public:

        class Row {
        public:
            explicit Row(sqlite3_stmt *stmt) : stmt(stmt), index(0) {}

            template<typename T>
            T get() {
                T value;
                // Currently, we only support int, double, and text columns.
                if constexpr (std::is_same_v<T, int>) {
                    value = sqlite3_column_int(stmt, index);
                } else if constexpr (std::is_same_v<T, double>) {
                    value = sqlite3_column_double(stmt, index);
                } else if constexpr (std::is_same_v<T, std::string>) {
                    value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, index));
                } else {
                    static_assert(sizeof(T) == 0, "Unsupported type");
                }
                ++index;
                return value;
            }

            bool hasNext() const {
                return sqlite3_step(stmt) == SQLITE_ROW;
            }

        private:
            sqlite3_stmt *stmt;
            int index;
        };

        explicit SQLiteDB(std::string filename) : filename(std::move(filename)) {
            connect();
        }

        virtual ~SQLiteDB() {
            disconnect();
        }

        bool connect();

        bool execute(const std::string &sql);

        Row select(const std::string &sql);

        void disconnect() noexcept;

        bool isConnected() const {
            return db;
        }

        bool hasError() const {
            return errMsg != nullptr;
        }

        const char *getError() const {
            return const_cast<const char*>(errMsg);
        }

    private:
        std::string filename;
        sqlite3 *db = nullptr;
        char *errMsg = nullptr;
    };
}
#endif //TINYTP_DB_H
