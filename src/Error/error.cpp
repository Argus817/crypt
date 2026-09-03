#include <iostream>

#include "error.hpp"

using namespace std;

void throwErrorCode(ErrorCodes code, string_view context) {
    cerr << Errors[code] << context << endl;
    throw code;
}