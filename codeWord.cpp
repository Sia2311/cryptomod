#include "CipherInterface.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <set>
#include <sstream>
#include <iomanip>
#include <cstring>
using namespace std;

// строим новый алфавит по ключу
// Строим новый алфавит на основе ключа
vector<uint8_t> buildingAlphabet(const string& key) {
    vector<uint8_t> base(256);
    for (int i = 0; i < 256; ++i) {
        base[i] = static_cast<uint8_t>(i);
    }

    set<uint8_t> seen;
    vector<uint8_t> result;

    for (char c : key) {
        uint8_t b = static_cast<uint8_t>(c);
        if (!seen.count(b)) {
            result.push_back(b);
            seen.insert(b);
        }
    }

    for (uint8_t b : base) {
        if (!seen.count(b)) {
            result.push_back(b);
        }
    }

    return result;
}

// Перевод строки в HEX без sprintf
string to_hex(const string& message) {
    string output;
    for (unsigned char i : message) {
        int high = i / 16;
        int low = i % 16;

        char first = (high < 10) ? ('0' + high) : ('A' + high - 10);
        char second = (low < 10) ? ('0' + low) : ('A' + low - 10);

        output += first;
        output += second;
    }
    return output;
}

// Обратное преобразование из HEX в строку
string from_hex(const string& hex) {
    string result;
    if (hex.size() % 2 != 0) return "";

    for (size_t i = 0; i < hex.size(); i += 2) {
        char first = hex[i];
        char second = hex[i + 1];

        int high = (first >= 'A') ? (first - 'A' + 10) : (first - '0');
        int low = (second >= 'A') ? (second - 'A' + 10) : (second - '0');

        result += static_cast<char>((high << 4) + low);
    }
    return result;
}

extern "C" {

const char* encrypt(const char* text, const char* key) {
    if (!text || !key) return nullptr;

    vector<uint8_t> newAlphabet = buildingAlphabet(key);
    uint8_t encrypt_map[256];
    for (int i = 0; i < 256; ++i) {
        encrypt_map[i] = newAlphabet[i];
    }

    string input(text);
    string encrypted;
    encrypted.reserve(input.size());

    for (unsigned char c : input) {
        encrypted.push_back(encrypt_map[c]);
    }

    string hex = to_hex(encrypted);
    return strdup(hex.c_str());
}

const char* decrypt(const char* hexText, const char* key) {
    if (!hexText || !key) return nullptr;

    string Byte = from_hex(hexText);
    if (Byte.empty()) return nullptr;

    vector<uint8_t> Alphabet = buildingAlphabet(key);
    uint8_t decryptMap[256];
    for (int i = 0; i < 256; ++i) {
        decryptMap[Alphabet[i]] = static_cast<uint8_t>(i);
    }

    string result;
    result.reserve(Byte.size());

    for (unsigned char c : Byte) {
        result.push_back(decryptMap[c]);
    }

    return strdup(result.c_str());
}

const char* get_name() {
    return "Шифр кодового слова";
}

const char* get_description() {
    return R"(Шифр кодового слова — байтовая перестановка, основанная на введённом ключе.
На его основе строится новый алфавит: сначала добавляются уникальные символы ключа в порядке появления,
затем оставшиеся байты (0–255), не входящие в ключ.

Каждый символ исходного текста заменяется на байт из нового алфавита с тем же индексом.

Пример:
Ключ:  SIMPLE
Первые байты нового алфавита:  'S', 'I', 'M', 'P', 'L', 'E', ...

Текст:  HELLO
Результат (в HEX): 42 3F 47 47 4B

Для расшифровки используется тот же ключ — алфавит восстанавливается в исходном порядке.

Ключ: строка (ASCII или UTF-8). Регистр имеет значение. Результат шифрования выводится в виде HEX.)";
}


bool returnHex() {
    return true;
}

}
