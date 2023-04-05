#include <tinytp/prio.h>
#include <tinytp/model.h>
#include <gmock/gmock.h>
#include <filesystem>
#include <fstream>

using namespace testing;
using namespace tinytp;

namespace fs = std::filesystem;

class PrioTestSuite : public Test {
protected:
    void SetUp() override {
        tempFile = fs::temp_directory_path() / "test.txt";
        if (fs::exists(tempFile)) {
            fs::remove(tempFile);
        }
    }

    void TearDown() override {
        if (fs::exists(tempFile)) {
            fs::remove(tempFile);
        }
    }

    fs::path tempFile;
};

TEST_F(PrioTestSuite, EmptyChangeset) {
    
}
