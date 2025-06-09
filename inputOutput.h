#pragma once
#include <string>

#include "module_load.h"

struct InputData
{
    std::string text;
    std::string key;
    bool encrypt;
};

InputData getUserInput(const CipherPlugin& cipher);
bool outputResult(const std::string& result, bool encrypt);