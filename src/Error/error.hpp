#include <string_view>

enum ErrorCodes {
    INPUT_FILE_ERROR,
    OUTPUT_FILE_ERROR,
    INVALID_DECRYPTION_FILE,
    INVALID_KEY_ERROR,
    INVALID_HEX_INPUT,
    INVALID_HEX_CHARACTER,
};

const inline char* Errors[] {
    "Failed to open Input File: ",
    "Failed to open Output File: ",
    "Can't decrypt file",
    "Invalid or wrong Key found",
    "Hex string length must be even",
    "Invalid hexadecimal character encountered",
};

void throwErrorCode(
    ErrorCodes code, 
    std::string_view context = ""
);