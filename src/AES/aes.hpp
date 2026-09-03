#include <vector>

void encrypt_block(
    std::vector <unsigned char>& key,
    unsigned char* plaintext
);
void decrypt_block(
    std::vector <unsigned char>& key,
    unsigned char* ciphertext
);