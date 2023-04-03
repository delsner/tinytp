#include <tinytp/util.h>
#include <fstream>

namespace tinytp {
    namespace string {
        std::vector<std::string> split(const std::string &str, const std::string &delim) {
            std::vector<std::string> tokens;
            size_t prev = 0, pos = 0;
            do {
                pos = str.find(delim, prev);
                if (pos == std::string::npos) pos = str.length();
                std::string token = str.substr(prev, pos - prev);
                if (!token.empty()) tokens.push_back(token);
                prev = pos + delim.length();
            } while (pos < str.length() && prev < str.length());
            return tokens;
        }

        std::string join(const std::vector<std::string> &vec, const std::string &delim) {
            std::string ret;
            for (const auto &s: vec) {
                if (!ret.empty())
                    ret += delim;
                ret += s;
            }
            return ret;
        }
    }

    namespace IO {
        fs::path writeFile(const std::string &content, const fs::path &file) {
            std::ofstream ofs(file);
            ofs << content;
            ofs.close();
            return file;
        }

        std::string readFile(const fs::path &file) {
            std::ifstream ifs(file);
            std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
            ifs.close();
            return content;
        }
    }
}