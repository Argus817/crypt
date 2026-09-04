#include <vector>
#include <memory>

#include "common.hpp"

#include "aes.hpp"

using namespace std;

AES::AES(vector <unsigned char>& key) : round_keys{ expand_key(key) } {}

void AES::encrypt_block(unsigned char* plaintext) {
    State state { bytes2matrix(plaintext) };
    add_round_key(state, round_keys[0]);
    sub_bytes(state);
    shift_rows(state);

    for (size_t i=1; i<n_rounds; i++) {
        mix_columns(state);
        add_round_key(state, round_keys[i]);
        sub_bytes(state);
        shift_rows(state);
    }

    add_round_key(state, round_keys[n_rounds]);
    matrix2bytes(state, plaintext);
}

void AES::decrypt_block(unsigned char* ciphertext) {
    State state { bytes2matrix(ciphertext) };
    add_round_key(state, round_keys[n_rounds]);

    for (size_t i = n_rounds-1; i>0; i--) {
        inv_shift_rows(state);
        sub_bytes(state, inv_s_box);
        add_round_key(state, round_keys[i]);
        inv_mix_columns(state);
    }

    inv_shift_rows(state);
    sub_bytes(state, inv_s_box);
    add_round_key(state, round_keys[0]);
    matrix2bytes(state, ciphertext);
}

