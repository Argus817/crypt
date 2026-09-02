#include "common.hpp"

std::vector <std::byte> decrypt(
    std::vector <std::byte>& key,
    std::vector <std::byte>& ciphertext
);

std::vector <std::byte> encrypt(
    std::vector <std::byte>& key,
    std::vector <std::byte>& plaintext
);