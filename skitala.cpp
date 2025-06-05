#include "CipherInterface.h"
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <cstdint>
using namespace std;

vector<string> utf8_split(const string& input) {
    vector<string> result;
    uint64_t i = 0;
    while (i < input.size()) {
        unsigned char c = input[i];
        uint64_t len = 1;

        if ((c & 0x80) == 0x00) len = 1;           // ASCII
        else if ((c & 0xE0) == 0xC0) len = 2;      // 2 байта
        else if ((c & 0xF0) == 0xE0) len = 3;      // 3 байта
        else if ((c & 0xF8) == 0xF0) len = 4;      // 4 байта

        result.push_back(input.substr(i, len));
        i += len;
    }
    return result;
}

//заполняем по строка читаем по столбцам
string encrypt_skitale(const string& text, int columns) {
    if (text.empty() || columns <= 0) return "";

    vector<string> letters = utf8_split(text);
    int rows = static_cast<int>(ceil((double)letters.size() / columns));
    vector<vector<string>> grid(rows, vector<string>(columns, " "));

    int index = 0;
    for (int r = 0; r < rows && index < letters.size(); ++r) {
        for (int c = 0; c < columns && index < letters.size(); ++c) {
            grid[r][c] = letters[index++];
        }
    }

    string result;
    for (int c = 0; c < columns; ++c) {
        for (int r = 0; r < rows; ++r) {
            result += grid[r][c];
        }
    }
    return result;
}

// заполняем по столбцам, читаем по строкам
string decrypt_skitale(const string& text, int columns) {
    vector<string> letters = utf8_split(text);
    int rows = static_cast<int>(ceil((double)letters.size() / columns));
    int totalCells = rows * columns;
    int pad = totalCells - letters.size();

    vector<vector<string>> grid(rows, vector<string>(columns, " "));

    int index = 0;
    for (int c = 0; c < columns && index < letters.size(); ++c) {
        for (int r = 0; r < rows && index < letters.size(); ++r) {
            if (c >= columns - pad && r == rows - 1) continue;
            grid[r][c] = letters[index++];
        }
    }

    string result;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            result += grid[r][c];
        }
    }
    return result;
}

extern "C"{
    const char* encrypt(const char* text, const char* key){
        if(!text || !key) return nullptr;

        int columns;
        try{
            columns = stoi(key);
        } catch(...){
            fprintf(stderr, "[Скитала] Ключ должен быть число!\n");
            return nullptr;
        }
        string result = encrypt_skitale(text, columns);
        return strdup(result.c_str()); // кидаем в кучу
    }
    const char* decrypt(const char* text, const char* key){

        int columns;
        try{
            columns = stoi(key);
        }catch (...){
            fprintf(stderr, "[Скитал] Ключ должен быть целым числом!");
            return nullptr;
        }
        string result = decrypt_skitale(text,columns);
        return strdup(result.c_str());
    }
    const char* get_name() {
        return "Скитала (перестановка)";
    }
    const char* get_description() {
        return R"(Скитала — античный шифр перестановки, использовавшийся в Спарте.
    Текст записывается по строкам в таблицу с фиксированным числом столбцов (ключ),
    затем считывается по столбцам, формируя зашифрованную последовательность.
    
    Пример: при ключе 4 и тексте "ПРИВЕТМИР" таблица будет:
    
    П Р И В
    Е Т М И
    Р _ _ _
    
    Шифр: ПЕРРТТИИМВ
    
    Ключ: целое число — количество столбцов.)";
    }
    
}