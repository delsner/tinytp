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
            explicit Row(sqlite3_stmt *stmt) : stmt(stmt), index(0) {
                columns = sqlite3_column_count(stmt);
            }

            virtual ~Row() {
                sqlite3_finalize(stmt);
            }

            template<typename T>
            T get() {
                T value;
                if constexpr (std::is_same_v<T, int>) {
                    value = sqlite3_column_int(stmt, index);
                } else if constexpr (std::is_same_v<T, double>) {
                    value = sqlite3_column_double(stmt, index);
                } else if constexpr (std::is_same_v<T, std::string>) {
                    value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, index));
                } else {
                    static_assert(sizeof(T) == 0, "Unsupported type");
                }
                index = (index + 1) % columns;
                return value;
            }

            bool hasNext() {
                index = 0;
                return sqlite3_step(stmt) == SQLITE_ROW;
            }

        private:
            sqlite3_stmt *stmt;
            int index;
            int columns;
        };

        explicit SQLiteDB(std::string filename) : filename(std::move(filename)) {
        }

        virtual ~SQLiteDB() {
            std::cout << "Destructing SQLiteDB" << std::endl;
            disconnect();
        }

        bool connect() noexcept;

        bool execute(const std::string &sql);

        Row select(const std::string &sql);

        void disconnect() noexcept;

        [[nodiscard]] bool isConnected() const {
            return db;
        }

        [[nodiscard]] bool hasError() const {
            return errMsg != nullptr;
        }

        [[nodiscard]] const char *getError() const {
            return const_cast<const char *>(errMsg);
        }

    private:
        std::string filename;
        sqlite3 *db = nullptr;
        char *errMsg = nullptr;
    };
}
#endif //TINYTP_DB_H
