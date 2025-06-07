#include <iostream>
#include <limits>
#include "utils.h"
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <stdexcept>
using namespace std;

string readBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        throw runtime_error("Ошибка чтения бинарного файла: " + filename);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool writeBinaryFile(const string& filename, const string& data) {
    ofstream file(filename, ios::binary);
    if (!file) {
        throw runtime_error("Ошибка записи бинарного файла: " + filename);
    }
    file.write(data.data(), data.size());
    return true;
}

string readFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Не удалось открыть файл: " + filename);
    }
    stringstream copy;
    copy << file.rdbuf();
    return copy.str();
}

bool writeFile(const string& filename, const string& text) {
    ofstream file(filename);
    if (!file) {
        throw runtime_error("Не удалось открыть файл: " + filename);
    }
    file << text;
    return true;
}

string fromHex(const string& hex) {
    string result;
    if (hex.size() % 2 != 0) return "";

    for (uint64_t i = 0; i < hex.size(); i += 2) {
        unsigned int byte;
        if (sscanf(hex.substr(i, 2).c_str(), "%02X", &byte) != 1)
            return "";
        result += static_cast<char>(byte);
    }
    return result;
}

void userPause() {
    cout << "\nНажмите Enter для возврата...";
    cin.ignore();
    cin.get();
}

void clearCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
