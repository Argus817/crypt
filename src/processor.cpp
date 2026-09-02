#include "processor.hpp"

#include <filesystem>
#include <cassert>
#include <iostream>

using namespace std;
namespace fs = filesystem;

void process_file(fs::path filepath, string_view action) {
    assert(fs::exists(filepath) && fs::is_regular_file(filepath));

    vector <byte> key(16, static_cast<byte>(4)), ciphertext(16, static_cast<byte>(5));

    if (action == "encrypt") {
        cout << "Starting encryption process...\n";
        vector <byte> plaintext = encrypt(key, ciphertext);
        for (int i=0; i<16; i++) {
            cout << static_cast<int>(plaintext[i]) << ' ';
        }
        cout << endl;
    } else {
        cout << "Starting decryption process...\n";
        vector <byte> plaintext = decrypt(key, ciphertext);
        for (int i=0; i<16; i++) {
            cout << static_cast<int>(plaintext[i]) << ' ';
        }
        cout << endl;
    }

    cout << filepath << ' ' << action << endl;
}

void process_dir(fs::path dirPath, string_view action) {
    assert(fs::exists(dirPath) && fs::is_directory(dirPath));

    for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
        process_file(entry.path(), action);
    }
}