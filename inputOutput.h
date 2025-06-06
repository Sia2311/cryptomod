#pragma once
#include <string>
#include "module_load.h"

using namespace std;

struct InputData {
    string text;
    string key;
    bool encrypt;
};

InputData getUserInput(const CipherPlugin& cipher);
bool outputResult(const string& result, const CipherPlugin& cipher, bool encrypt);