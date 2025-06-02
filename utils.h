#pragma once

#include <string>
using namespace std;

string readFile(const string& filename);
bool writeFile(const string& filename, const string& text);
void pause();
void clearCin();