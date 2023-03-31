#include <sqlite3.h>
#include <iostream>

namespace tinytp::db {
    bool connect() {
        std::cout << "sqllite_version=" << sqlite3_libversion() << std::endl;
        return true;
    }
}