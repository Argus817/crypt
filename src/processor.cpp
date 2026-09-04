#include <filesystem>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>
#include <chrono>

#include "AES/aes.hpp"
#include "Error/error.hpp"
#include "AES/constants.hpp"
#include "AES/base64.hpp"

#include "processor.hpp"

using namespace std;
namespace fs = filesystem;

inline vector <unsigned char> KEY(BLOCKSIZE, static_cast<unsigned char>(69));

fs::path encrypt_filename(const fs::path& filepath, AES& cipher) {
    string filename = filepath.filename().string();
    size_t remainder { filename.length() % BLOCKSIZE };
    size_t required { BLOCKSIZE - remainder };

    filename.resize(filename.length()+required, static_cast<char>(required));

    for (size_t i=0; i<filename.length(); i += BLOCKSIZE) {
        cipher.encrypt_block(reinterpret_cast<unsigned char*>(&filename[i]));
    }

    return filepath.parent_path() / (base64::to_base64(filename)+".enc"s);
}

void encrypt_file(const fs::path& filepath, AES& cipher) {
    fs::path newFilepath = encrypt_filename(filepath, cipher);

    ifstream infile(filepath, ios::binary);
    if (!infile.is_open()) {
        throwErrorCode(ErrorCodes::INPUT_FILE_ERROR, filepath.string());
    }

    ofstream outfile(newFilepath, ios::binary);
    if (!outfile.is_open()) {
        throwErrorCode(ErrorCodes::OUTPUT_FILE_ERROR, newFilepath.string());
    }

    vector <unsigned char> buffer(BUFFSIZE);

    while (infile) {
        infile.read(reinterpret_cast<char*>(buffer.data()), BUFFSIZE);
        size_t bytesRead { static_cast<size_t>(infile.gcount()) };

        if (bytesRead == 0) break;

        size_t totalBytes = bytesRead;

        if (bytesRead < BUFFSIZE || infile.peek() == EOF) {
            size_t remainder { bytesRead % BLOCKSIZE };
            size_t required { BLOCKSIZE - remainder };
            totalBytes += required;

            if (totalBytes > buffer.size()) {
                buffer.resize(totalBytes);
            }

            for (size_t i {bytesRead}; i < totalBytes; i++) {
                buffer[i] = static_cast<unsigned char>(required);
            }
        }

        for (size_t i=0; i<totalBytes; i += BLOCKSIZE) {
            cipher.encrypt_block(&buffer[i]);
        }
        outfile.write(reinterpret_cast<char*>(buffer.data()), totalBytes);
    }

    infile.close();
    fs::remove(filepath);
}

fs::path decrypt_filename(const fs::path& filepath, AES& cipher) {
    auto decoded = base64::from_base64(filepath.stem().c_str());

    if (decoded.length() % BLOCKSIZE != 0) {
        throwErrorCode(ErrorCodes::INVALID_DECRYPTION_FILE);
    }

    size_t iterations = decoded.length() / BLOCKSIZE;
    size_t padding = 0;

    for (size_t i { 0 }; i < iterations; ++i) {
        cipher.decrypt_block(reinterpret_cast<unsigned char*>(&decoded[(iterations-1-i)*BLOCKSIZE]));

        if (i == 0) {
            padding = decoded[decoded.length()-1];
            for (size_t i=1; i<=padding; i++) {
                if (decoded[decoded.length() - padding] != static_cast<char>(padding)) {
                    throwErrorCode(ErrorCodes::INVALID_KEY_ERROR);
                }
            }
        }
    }

    decoded.resize(decoded.length() - padding);

    return filepath.parent_path() / decoded;
}

void decrypt_file(const fs::path& filepath, AES& cipher) {
    fs::path newFilepath = decrypt_filename(filepath, cipher);

    ifstream infile(filepath, ios::binary);
    if (!infile.is_open()) {
        throwErrorCode(ErrorCodes::INPUT_FILE_ERROR, filepath.string());
    }

    ofstream outfile(newFilepath, ios::binary);
    if (!outfile.is_open()) {
        throwErrorCode(ErrorCodes::OUTPUT_FILE_ERROR, newFilepath.string());
    }

    vector <unsigned char> buffer(BUFFSIZE);

    uintmax_t fileSize = fs::file_size(filepath);
    size_t iterations = fileSize / BUFFSIZE;
    if (fileSize % BUFFSIZE) iterations++;

    while (iterations--) {
        infile.read(reinterpret_cast<char*>(buffer.data()), BUFFSIZE);
        size_t bytesRead { static_cast<size_t>(infile.gcount()) };

        for (size_t i=0; i<bytesRead; i += BLOCKSIZE) {
            cipher.decrypt_block(&buffer[i]);
        }

        if (iterations == 0) {
            bytesRead -= static_cast<size_t>(buffer[bytesRead-1]);
        }

        outfile.write(reinterpret_cast<char*>(buffer.data()), bytesRead);
    }
    infile.close();
    fs::remove(filepath);
}

void process_file(const fs::path& filepath, string_view action, AES& cipher) {
    auto start = chrono::high_resolution_clock::now();
    assert(fs::exists(filepath) && fs::is_regular_file(filepath));
    string_view ext { filepath.extension().c_str() };

    if (action == "encrypt" && ext != ".enc") {
        cout << "Encrypting " << filepath.filename() << endl; 
        encrypt_file(filepath, cipher);
    } else if (action == "decrypt" && ext == ".enc") {
        cout << "Decrypting " << filepath.filename() << endl;
        decrypt_file(filepath, cipher);
    }
    else {
        return;
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);
    cout << filepath.filename() << " " << action << "ed in " << duration << "\n" << endl;
}

void process_dir(fs::path& dirPath, string_view action) {
    assert(fs::exists(dirPath) && fs::is_directory(dirPath));

    AES cipher(KEY);

    for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
        if (fs::is_regular_file(entry.path())) {
            try {
                process_file(entry.path(), action, cipher);
            }
            catch (const ErrorCodes err) {
                cerr << entry.path().filename() << " failed!!" << endl;
            }
        }
    }
}