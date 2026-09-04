#include <vector>

class AES {
private:
    std::vector <std::vector <std::vector <int>>> round_keys;

public:
    AES(std::vector <unsigned char>& key);

    void encrypt_block(
        unsigned char* plaintext
    );
    void decrypt_block(
        unsigned char* ciphertext
    );
};