#ifndef TINYTP_RUNNER_H
#define TINYTP_RUNNER_H

#include <filesystem>
#include <string>
#include <utility>

#include <tinytp/db.h>

namespace tinytp {
    /// Base interface for runners that can be configured using CLI parameters.
    class TinyTPRunner {
    public:
        explicit TinyTPRunner(const SQLiteDB& db) : db(db) {};

        virtual ~TinyTPRunner() = default;

        virtual int run() = 0;

    protected:
        SQLiteDB db;
    };
}
#endif //TINYTP_RUNNER_H
