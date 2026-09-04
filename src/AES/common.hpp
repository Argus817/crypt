#pragma once

#include <vector>
#include <array>

#include "constants.hpp"

State bytes2matrix(
    unsigned char* text
);
void matrix2bytes(
    State& matrix,
    unsigned char* text
);
void shift_rows(State& s);
void inv_shift_rows(State& s);
void mix_columns(State& s);
void inv_mix_columns(State& s);
std::vector <std::vector <std::vector <int>>> expand_key(
    std::vector <unsigned char>& master_key
);
void add_round_key(
    State& s, 
    std::vector <std::vector <int>>& k
);
void sub_bytes(
    State& s, 
    const std::array <int, 16*16> sbox = s_box
);