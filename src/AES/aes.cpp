#include "aes.hpp"

#include <vector>

using namespace std;

vector <byte> decrypt(vector <byte>& key, vector <byte>& ciphertext) {
    std::vector <std::vector <std::vector <int>>> round_keys { expand_key(key) };

    std::vector <std::vector <int>> state { bytes2matrix(ciphertext) };
    add_round_key(state, round_keys[n_rounds]);

    for (int i = n_rounds-1; i>0; i--) {
        inv_shift_rows(state);
        sub_bytes(state, inv_s_box);
        add_round_key(state, round_keys[i]);
        inv_mix_columns(state);
    }

    inv_shift_rows(state);
    sub_bytes(state, inv_s_box);
    add_round_key(state, round_keys[0]);
    return matrix2bytes(state);
}

vector <byte> encrypt(vector <byte>& key, vector <byte>& plaintext) {
    std::vector <std::vector <std::vector <int>>> round_keys { expand_key(key) };

    std::vector <std::vector <int>> state { bytes2matrix(plaintext) };
    add_round_key(state, round_keys[0]);
    sub_bytes(state);
    shift_rows(state);

    for (int i=1; i<n_rounds; i++) {
        mix_columns(state);
        add_round_key(state, round_keys[i]);
        sub_bytes(state);
        shift_rows(state);
    }

    add_round_key(state, round_keys[n_rounds]);
    return matrix2bytes(state);
}