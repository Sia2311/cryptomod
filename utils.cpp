#include <iostream>
#include <limits>
#include "utils.h"
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

// Конвертация строки в HEX
string stringToHex(const string& input) {
    ostringstream oss;
    for (unsigned char c : input) {
        oss << setw(2) << setfill('0') << hex << uppercase << static_cast<int>(c);
    }
    return oss.str();
}

//конвертация HEX в строку байтов
string hexToString(const string& hex) {
    string output;
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        char byte = static_cast<char>(strtol(byteString.c_str(), nullptr, 16));
        output.push_back(byte);
    }
    return output;
}

// Чтение бинарного файла
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

// Запись бинарного файла
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

void pause() {
    cout << "\nНажмите Enter для возврата...";
    cin.ignore();
    cin.get();
}

void clearCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}