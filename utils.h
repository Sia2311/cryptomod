#pragma once

#include <string>

std::string readBinaryFile(const std::string& filename);
bool writeBinaryFile(const std::string& filename, const std::string& data);

std::string readFile(const std::string& filename);
bool writeFile(const std::string& filename, const std::string& text);

std::string bytesToHex(const std::string& bytes);
std::string hexToBytes(const std::string& hex);

void userPause();
void clearCin();