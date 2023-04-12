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

        int levenshtein(const std::string& str1, const std::string& str2);
    }

    namespace IO {
        fs::path writeFile(const std::string &content, const fs::path &file);

        std::string readFile(const fs::path &file);

        std::vector<std::string> readLinesFromFile(const fs::path &file);
    }

}

#endif //TINYTP_UTIL_H
