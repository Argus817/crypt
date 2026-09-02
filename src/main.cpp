#include <iostream>
#include <string>
#include <filesystem>

#include "processor.hpp"

namespace fs = std::filesystem;
using namespace std;

void printUsage(const char* programName) {
    cerr << "Usage: " << programName << " <encrypt|decrypt> <target_directory>\n" << "Example: " << programName << " encrypt ./my_folder\n";
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage(argv[0]);
        return 1;
    }

    string action(argv[1]);
    if (action != "encrypt" && action != "decrypt") {
        cerr << "Error: Invalid action '" << action << "'. Must be 'encrypt' or 'decrypt'.\n\n";
        printUsage(argv[0]);
        return 1;
    }

    fs::path targetDir(argv[2]);
    if (!fs::exists(targetDir)) {
        cerr << "Error: The path '" << targetDir.string() << "' does not exist.\n";
        return 1;
    }
    if (!fs::is_directory(targetDir)) {
        cerr << "Error: The path '" << targetDir.string() << "' is not a directory.\n";
        return 1;
    }

    cout << "Action verified: " << action << "\n";
    cout << "Target folder verified: " << fs::absolute(targetDir).string() << "\n";

    process_dir(targetDir, action);

    return 0;
}
