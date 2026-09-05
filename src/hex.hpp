#include <string>
#include <array>

constexpr std::array<char, 16> lut { 
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' 
};

std::string ucstrToHexString(size_t n, unsigned char* begin);
std::string uchexstrToString(size_t n, const unsigned char* begin);