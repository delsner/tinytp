#include <iostream>
#include <tinytp/git.h>

int main(int argc, char* argv[]) {
    std::cout << "Hello=" << tinytp::git::diff() << "\n" << std::endl;
    return 0;
}