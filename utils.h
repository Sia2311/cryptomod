#pragma once

#include <string>
using namespace std;

string stringToHex(const string& input);

string hexToString(const string& hex);

string readBinaryFile(const string& filename);

bool writeBinaryFile(const string& filename, const string& data);

string readFile(const string& filename);

bool writeFile(const string& filename, const string& text);

string fromHex(const string& hex);

void userPause();
void clearCin();