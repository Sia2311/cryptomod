#pragma once
#include <string>
#include "module_load.h"

struct InputData {
    std::string text;
    std::string key;
    bool encrypt;
};

InputData getUserInput(const CipherPlugin& cipher, const std::string& defaultKey);
bool outputResult(const std::string& result, const CipherPlugin& cipher, bool encrypt);
