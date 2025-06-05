#include <random>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>
using namespace std;

string generateRandomKey() {
    const vector<string> charset = {
        "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
        "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
        "0","1","2","3","4","5","6","7","8","9",
        "!","@","#","$","%","^","&","*","(",")","_","-","+","=","{","}","[","]","|",";",":",",",".","<",">","?","/",
        "😀","😁","😂","🤣","😅","😊","😎","😍","😘","🥰","😇","🤖","💡","🔥","🌍","🌈","⚡️","★","✓","♻️","∞"
    };

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> lenDist(10, 100);
    uniform_int_distribution<> charDist(0, charset.size() - 1);

    uint64_t length = lenDist(gen);
    string key;
    key.reserve(length * 2);  // запас с учётом многобайтовых символов

    for (uint64_t i = 0; i < length; ++i) {
        key += charset[charDist(gen)];
    }

    return key;
}

string autoGenerateKey(const string& cipherName, uint64_t textLength) {
    if (cipherName.find("Скитала") != string::npos) {
        int guess = clamp((int)sqrt(textLength), 2, 100);
        return to_string(guess);
    } else {
        return generateRandomKey();  
    }
}

