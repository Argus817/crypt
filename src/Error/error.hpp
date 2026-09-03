#include <string_view>

enum ErrorCodes {
    INPUT_FILE_ERROR,
    OUTPUT_FILE_ERROR,
};

const inline char* Errors[] {
    "Failed to open Input File: ",
    "Failed to open Output File: ",
};

void throwErrorCode(
    ErrorCodes code, 
    std::string_view context = ""
);