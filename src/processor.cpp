#include <filesystem>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>

#include "AES/aes.hpp"
#include "Error/error.hpp"
#include "AES/constants.hpp"

#include "processor.hpp"

using namespace std;
namespace fs = filesystem;

inline vector <unsigned char> KEY(BLOCKSIZE, static_cast<unsigned char>(69));

void encrypt_file(fs::path filepath, vector <unsigned char>& key) {
    fs::path newFilepath = filepath.parent_path() / ("enc_"s+filepath.stem().string()+filepath.extension().string());

    ifstream infile(filepath, ios::binary);
    if (!infile.is_open()) {
        throwErrorCode(ErrorCodes::INPUT_FILE_ERROR, filepath.string());
    }

    ofstream outfile(newFilepath, ios::binary);
    if (!outfile.is_open()) {
        throwErrorCode(ErrorCodes::OUTPUT_FILE_ERROR, newFilepath.string());
    }

    vector <unsigned char> buffer(BUFFSIZE);
    AES cipher(key);

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

inline fs::path remove_filename_prefix(const fs::path& full_path, std::string_view prefix)
{
    std::string filename = full_path.filename().string();

    if (filename.rfind(prefix, 0) == 0)
    {
        filename.erase(0, prefix.length());
    }

    return full_path.parent_path() / filename;
}

void decrypt_file(fs::path filepath, vector <unsigned char>& key) {
    fs::path newFilepath = remove_filename_prefix(filepath, "enc_");

    ifstream infile(filepath, ios::binary);
    if (!infile.is_open()) {
        throwErrorCode(ErrorCodes::INPUT_FILE_ERROR, filepath.string());
    }

    ofstream outfile(newFilepath, ios::binary);
    if (!outfile.is_open()) {
        throwErrorCode(ErrorCodes::OUTPUT_FILE_ERROR, newFilepath.string());
    }

    vector <unsigned char> buffer(BUFFSIZE);
    AES cipher(key);

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

void process_file(fs::path filepath, string_view action) {
    assert(fs::exists(filepath) && fs::is_regular_file(filepath));
    string_view filename { filepath.stem().c_str() };

    if (action == "encrypt" && !filename.starts_with("enc_")) {
        cout << "Encrypting " << filepath.filename() << endl; 
        encrypt_file(filepath, KEY);
    } else if (action == "decrypt" && filename.starts_with("enc_")) {
        cout << "Decrypting " << filepath.filename() << endl;
        decrypt_file(filepath, KEY);
    }
}

void process_dir(fs::path dirPath, string_view action) {
    assert(fs::exists(dirPath) && fs::is_directory(dirPath));

    for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
        if (fs::is_regular_file(entry.path()))
            process_file(entry.path(), action);
    }
    cerr << "\nWorked in " << 1000*((double)clock())/(double)CLOCKS_PER_SEC<< "ms \n";
}