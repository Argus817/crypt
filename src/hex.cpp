#include <string>
#include <cassert>

#include "Error/error.hpp"

#include "hex.hpp"

using namespace std;

string ucstrToHexString(size_t n, unsigned char* begin) {
    string res(2*n, '0');

    for (size_t i { 0 }; i < n; ++i) {
        unsigned char byte { begin[i] };

        res[2*i] = lut[byte >> 4];
        res[2*i+1] = lut[byte & 0x0F];
    }
    return res;
}

inline unsigned char hexCharToNibble(unsigned char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    throwErrorCode(ErrorCodes::INVALID_HEX_CHARACTER);
    return 0;
}

string uchexstrToString(size_t n, const unsigned char* begin) {
    if (n % 2 != 0) {
        throwErrorCode(ErrorCodes::INVALID_HEX_INPUT);
    }

    size_t numBytes { n / 2 };
    string res(numBytes, 'x');

    for (size_t i { 0 }; i < numBytes; ++i) {
        unsigned char highNibble { hexCharToNibble(begin[2 * i]) };
        unsigned char lowNibble  { hexCharToNibble(begin[2 * i + 1]) };

        res[i] = static_cast<char>((highNibble << 4) | lowNibble);
    }

    return res;
}
