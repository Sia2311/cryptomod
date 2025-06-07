#include "CipherInterface.h"
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <stdexcept>
#include <cctype> // для isdigit

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

string encrypt_skitale(const string& text, int columns) {
    if (columns < 2) {
        throw invalid_argument("[Скитала] Ключ должен быть положительным числом и больше еденицы!");
    }
    if (text.empty()) return "";

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

string decrypt_skitale(const string& text, int columns) {
    if (columns < 2) {
        throw invalid_argument("[Скитала] Ключ должен быть положительным числом и больше еденицы!");
    }
    if (text.empty()) return "";

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

bool is_positive_integer(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

extern "C" {

const char* encrypt(const char* text, const char* key) {
    if (!text || !key) return nullptr;

    string keyStr(key);
    if (!is_positive_integer(keyStr)) {
        fprintf(stderr, "[Скитала] Ключ должен быть числом!\n");
        return nullptr;
    }

    int columns = stoi(keyStr);

    try {
        string result = encrypt_skitale(text, columns);
        return strdup(result.c_str());
    } catch (const invalid_argument& e) {
        fprintf(stderr, "%s\n", e.what());
        return nullptr;
    }
}

const char* decrypt(const char* text, const char* key) {
    if (!text || !key) return nullptr;

    string keyStr(key);
    if (!is_positive_integer(keyStr)) {
        fprintf(stderr, "[Скитала] Ключ должен быть числом!\n");
        return nullptr;
    }

    int columns = stoi(keyStr);

    try {
        string result = decrypt_skitale(text, columns);
        return strdup(result.c_str());
    } catch (const invalid_argument& e) {
        fprintf(stderr, "%s\n", e.what());
        return nullptr;
    }
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

Шифр: ПЕРРТ ИМ ВИ


Ключ: целое число — количество столбцов.)";
}

}
