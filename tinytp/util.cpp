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

        /// Computes the levenshtein distance using dynamic programming.
        /// Partly copied from: https://github.com/guilhermeagostinelli/levenshtein
        int levenshtein(const std::string &str1, const std::string &str2) {
            int size1 = static_cast<int>(str1.size());
            int size2 = static_cast<int>(str2.size());
            auto matrix = std::vector<std::vector<int>>(size1 + 1, std::vector<int>(size2 + 1, 0));

            // If one of the words has zero length, the distance is equal to the size of the other word.
            if (size1 == 0)
                return size2;
            if (size2 == 0)
                return size1;

            // Sets the first row and the first column of the verification matrix with the numerical order from 0 to the length of each word.
            for (int i = 0; i <= size1; i++)
                matrix[i][0] = i;
            for (int j = 0; j <= size2; j++)
                matrix[0][j] = j;

            for (int i = 1; i <= size1; i++) {
                for (int j = 1; j <= size2; j++) {
                    // Sets the modification cost.
                    // 0 means no modification (i.e. equal letters) and 1 means that a modification is needed (i.e. unequal letters).
                    int cost = (str2[j - 1] == str1[i - 1]) ? 0 : 1;

                    // Sets the current position of the matrix as the minimum value between a (deletion), b (insertion) and c (substitution).
                    // a = the upper adjacent value plus 1: matrix[i - 1][j] + 1
                    // b = the left adjacent value plus 1: matrix[i][j - 1] + 1
                    // c = the upper left adjacent value plus the modification cost: matrix[i - 1][j - 1] + cost
                    matrix[i][j] = std::min(
                            std::min(matrix[i - 1][j] + 1, matrix[i][j - 1] + 1),
                            matrix[i - 1][j - 1] + cost
                    );
                }
            }

            // The last position of the matrix will contain the Levenshtein distance.
            return matrix[size1][size2];
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

        std::vector<std::string> readLinesFromFile(const fs::path &file) {
            std::ifstream ifs(file);
            std::vector<std::string> lines;
            if (ifs.is_open()) {
                std::string line;
                while (std::getline(ifs, line)) {
                    lines.push_back(line);
                }
                ifs.close();
            }
            return lines;
        }
    }
}