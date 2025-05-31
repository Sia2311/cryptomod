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

string to_hex(const string& message){
    string output;
    output.reserve(message.size()*2);
    for (unsigned char i : message ){
        int high = i/16; // старшие 4 бита
        int low = i%16; // младшие 4 бита

        char first = (high < 10) ? ('0' + high) : ('A' + high - 10);
        char second = (low <10) ? ('0'+ low) : ('A' + low - 10);

        output +=first;
        output+=second;
        }
        return output;
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

    //шифруем
    for (unsigned char c : input){
        encrypted.push_back(encrypt_map[c]);
    }
    string output = to_hex(encrypted);
    return strdup(output.c_str());
}

const char* decrypt (const char* text, const char* key){
    string hex(text);
    string Byte;

    // hex в байты
    for(int i = 0; i < hex.size(); i +=2){
        char high = hex[i];
        char low = hex[i + 1];

        int h = (high >= '0' && high <= '9') ? high - '0' : high - 'a' + 10;
        int l = (low >= '0' && low <= '9') ? low - '0' : low - 'a' + 10;

        Byte.push_back(static_cast<char>((h << 4) | 1));
    }
    //мастерим табличку дешифра
    vector<uint8_t> Alphabet = buildingAlphabet(key);
    uint8_t decryptMap[256];
    for(int i = 0; i < 256; ++i){
        decryptMap[Alphabet[i]] = static_cast<uint8_t>(i);
    }
    //дешифруем
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
    }