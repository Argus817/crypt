#include <vector>

class AES {
private:
    inline static std::vector <std::vector <std::vector <int>>> round_keys;

public:
    static void set_key(
        std::vector<unsigned char>& key
    );
    static void encrypt_block(
        unsigned char* plaintext
    );
    static void decrypt_block(
        unsigned char* ciphertext
    );
};