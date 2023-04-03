#ifndef TINYTP_UTIL_H
#define TINYTP_UTIL_H

#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace tinytp {
    namespace string {
        std::vector<std::string> split(const std::string &str, const std::string &delim);

        std::string join(const std::vector<std::string> &vec, const std::string &delim);
    }
    namespace IO {
        fs::path writeFile(const std::string &content, const fs::path &file);

        std::string readFile(const fs::path &file);
    }
}

#endif //TINYTP_UTIL_H
