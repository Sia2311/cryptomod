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
vector<uint8_t> buildingAlphabet (const string& key){
    // базовый аски
    vector<uint8_t> base(256);
    for(int i = 0; i < 256; ++i){
        base[i] = static_cast<uint8_t>(i);
    }
    set<uint8_t> see;
    vector<uint8_t> result; // надеюсь здесь когdа-то будет новый алфавит
    //тутутутуту поглядим на ключ и положим его в веткорок
    for (char c : key){
        uint8_t b = static_cast<uint8_t>(c);
        if(!see.count(b)){
            result.push_back(b);
            see.insert(b);
        }
    }
    //остальной алфавит
    for(uint8_t b : base){
        if(!see.count(b)){
            result.push_back(b);
        }
    }
    return result;
}

extern "C" {

const char* encrypt(const char* text, const char* key){
    if (!text || !key) return "";

    vector<uint8_t>  newAlphabet = buildingAlphabet(key);
    uint8_t encrypt_map[256]; // ключ байт а значение зашифрованный байт

    for(int i = 0; i < 256; ++i){
        encrypt_map[i] = newAlphabet[i];
    }

    string input(text);
    string encrypted;
    encrypted.reserve(input.size());

    // шифруем
    for (unsigned char c : input){
        encrypted.push_back(encrypt_map[c]);
    }

    return strdup(encrypted.c_str()); // возвращаем байты без hex
}

const char* decrypt(const char* text, const char* key){
    if (!text || !key) return "";

    string Byte(text);

    // мастерим табличку дешифра
    vector<uint8_t> Alphabet = buildingAlphabet(key);
    uint8_t decryptMap[256];
    for(int i = 0; i < 256; ++i){
        decryptMap[Alphabet[i]] = static_cast<uint8_t>(i);
    }

    // дешифруем
    string result;
    result.reserve(Byte.size());
    for(unsigned char c : Byte){
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
    return false;
}

}
