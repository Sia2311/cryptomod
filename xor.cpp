#include "CipherInterface.h"
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>  
#include <cstdint>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

using namespace std;

// преобразование бинарников в хекс
string to_hex(const string& message) {
    string output;
    for (unsigned char i : message) {
        int high = i / 16;
        int low = i % 16;

        output += (high < 10) ? ('0' + high) : ('A' + high - 10);
        output += (low < 10) ? ('0' + low) : ('A' + low - 10);
    }
    return output;
}

// преобразование хекс в байты
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

extern "C" {

EXPORT const char* encrypt(const char* text, const char* key) {

    uint64_t textLen = strlen(text);
    uint64_t keyLen = strlen(key);

    string result;
    result.resize(textLen);

    for (uint64_t i = 0; i < textLen; ++i) {
        result[i] = text[i] ^ key[i % keyLen];
    }

    string hexResult = to_hex(result);
    return strdup(hexResult.c_str());
}

EXPORT const char* decrypt(const char* hexText, const char* key) {

    string binary = fromHex(hexText);

    uint64_t keyLen = strlen(key);

    string result;
    result.resize(binary.size());

    for (uint64_t i = 0; i < binary.size(); ++i) {
        result[i] = binary[i] ^ key[i % keyLen];
    }

    return strdup(result.c_str());
}

EXPORT const char* get_name() {
    return "Xor шифрование";
}

EXPORT const char* get_description() {
    return R"(XOR — симметричный побитовый шифр, основанный на логической операции исключающего ИЛИ.
Каждый байт текста XOR ится с байтом ключа. Если ключ короче текста, он повторяется по кругу.

Результат шифрования в этой программе возвращается в виде HEX-строки.

Пример:
Текст:  HELLO
Ключ:   KEY
XOR (в HEX): 03 00 15 07 0A

Тот же ключ используется для дешифрования — повторный XOR возвращает исходный текст.

Ключ: строка (ASCII или UTF-8).)";
}

}
