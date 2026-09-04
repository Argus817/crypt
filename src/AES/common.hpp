#pragma once

#include <vector>
#include <array>

#include "constants.hpp"

std::vector <std::vector <int>> bytes2matrix(
    unsigned char* text
);
void matrix2bytes(
    std::vector <std::vector <int>>& matrix,
    unsigned char* text
);
void shift_rows(std::vector <std::vector <int>>& s);
void inv_shift_rows(std::vector <std::vector <int>>& s);
void mix_columns(std::vector <std::vector <int>>& s);
void inv_mix_columns(std::vector <std::vector <int>>& s);
std::vector <std::vector <std::vector <int>>> expand_key(
    std::vector <unsigned char>& master_key
);
std::vector <std::vector <std::vector <int>>> expand_key(
    std::vector <unsigned char>& master_key
);
void add_round_key(
    std::vector <std::vector <int>>& s, 
    std::vector <std::vector <int>>& k
);
void sub_bytes(
    std::vector <std::vector <int>>& s, 
    const std::array <int, 16*16> sbox = s_box
);