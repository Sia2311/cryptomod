#include <iostream>
#include <limits>
#include "utils.h"
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdint>
using namespace std;

// Чтение бинарника
string readBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Ошибка чтения бинарного файла: " << filename << endl;
        return "";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// запись бинарника
bool writeBinaryFile(const string& filename, const string& data) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Ошибка записи бинарного файла: " << filename << endl;
        return false;
    }
    file.write(data.data(), data.size());
    return true;
}

string readFile(const string& filename){
    ifstream file(filename);
    if(!file){
        cerr << "Не удалось открыть файл" <<  filename << endl;
        return "";
    }
    stringstream copy;
    copy << file.rdbuf();
    return copy.str();
}

bool writeFile(const string& filename, const string& text){
    ofstream file(filename);
    if(!file){
        cerr << "Не удалось открыть файл" << filename << endl;
        return false;
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

void pause() {
    cout << "\nНажмите Enter для возврата...";
    cin.ignore();
    cin.get();
}

void clearCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}