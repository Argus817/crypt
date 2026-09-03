#include <vector>
#include <cassert>

#include "common.hpp"

using namespace std;

vector <vector <int>> bytes2matrix(unsigned char* text) {
    vector <vector <int>> matrix(4, vector<int>(4));
    for (int i=0; i<16; i++) {
        matrix[i/4][i%4] = static_cast<int>(text[i]);
    }
    return matrix;
}

void matrix2bytes(vector <vector <int>>& matrix, unsigned char* text) {
    assert(matrix.size() == 4 && matrix[0].size() == 4);

    for (int i=0; i<16; i++) {
        text[i] = static_cast<unsigned char>(matrix[i/4][i%4]);
    }
}

void shift_rows(vector <vector <int>>& s) {
    int temp { s[0][1] };
    s[0][1] = s[1][1];
    s[1][1] = s[2][1];
    s[2][1] = s[3][1];
    s[3][1] = temp;

    swap(s[0][2], s[2][2]);
    swap(s[1][2], s[3][2]);

    temp = s[3][3];
    s[3][3] = s[2][3];
    s[2][3] = s[1][3];
    s[1][3] = s[0][3];
    s[0][3] = temp;
}

void inv_shift_rows(vector <vector <int>>& s) {
    int temp { s[3][1] };
    s[3][1] = s[2][1];
    s[2][1] = s[1][1];
    s[1][1] = s[0][1];
    s[0][1] = temp;

    swap(s[0][2], s[2][2]);
    swap(s[1][2], s[3][2]);

    temp = s[0][3];
    s[0][3] = s[1][3];
    s[1][3] = s[2][3];
    s[2][3] = s[3][3];
    s[3][3] = temp;
}

inline int xtime(int a) {
    return (a & 0x80)? (((a << 1) ^ 0x1B) & 0xFF) : (a << 1);
}

void mix_single_column(vector <int>& a) {
    int t { a[0] ^ a[1] ^ a[2] ^ a[3] };
    int u { a[0] };
    a[0] ^= t ^ xtime(a[0] ^ a[1]);
    a[1] ^= t ^ xtime(a[1] ^ a[2]);
    a[2] ^= t ^ xtime(a[2] ^ a[3]);
    a[3] ^= t ^ xtime(a[3] ^ u);
}

void mix_columns(vector <vector <int>>& s) {
    for (auto& x : s) {
        mix_single_column(x);
    }
}

void inv_mix_columns(vector <vector <int>>& s) {
    for (int i=0; i<4; i++) {
        int u { xtime(xtime(s[i][0] ^ s[i][2])) };
        int v { xtime(xtime(s[i][1] ^ s[i][3])) };
        s[i][0] ^= u;
        s[i][1] ^= v;
        s[i][2] ^= u;
        s[i][3] ^= v;
    }
    mix_columns(s);
}

vector <vector <vector <int>>> expand_key(vector <unsigned char>& master_key) {  //assume master_key is 16-bytes
    vector <vector <int>> key_columns { bytes2matrix(master_key.data()) };
    int iteration_size { 4 };

    int i { 1 };
    while (key_columns.size() < static_cast<size_t>((n_rounds + 1) * 4)) {
        vector<int> word = key_columns.back();

        if (key_columns.size() % iteration_size == 0) {
            int temp = word[0];
            word[0] = word[1];
            word[1] = word[2];
            word[2] = word[3];
            word[3] = temp;

            for (int k = 0; k < 4; k++) {
                word[k] = s_box[word[k]];
            }

            word[0] ^= r_con[i];
            i++;
        }

        const vector<int>& prev_word = key_columns[key_columns.size() - iteration_size];
        for (int k = 0; k < 4; k++) {
            word[k] ^= prev_word[k];
        }

        key_columns.push_back(word);
    }

    vector <vector <vector <int>>> round_keys;
    size_t num_round_keys = key_columns.size() / 4;

    for (size_t r = 0; r < num_round_keys; r++) {
        vector<vector<int>> matrix(4, vector<int>(4));
        for (int c = 0; c < 4; c++) {
            matrix[c] = key_columns[r * 4 + c];
        }
        round_keys.push_back(matrix);
    }

    return round_keys;
}

void add_round_key(vector <vector <int>>& s, vector <vector <int>>& k) {
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            s[i][j] ^= k[i][j];
        }
    }
}

void sub_bytes(vector <vector<int>>& s, const vector <int>& sbox) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            s[i][j] = sbox[s[i][j]];
        }
    }
}