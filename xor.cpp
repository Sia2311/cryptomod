#include "CipherInterface.h"
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>  

using namespace std;

extern "C" {
// Преобразование бинарных данных в HEX-строку
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


// Преобразование HEX-строки обратно в байты
string fromHex(const string& hex) {
    string result;
    if (hex.size() % 2 != 0) return "";

    for (size_t i = 0; i < hex.size(); i += 2) {
        unsigned int byte;
        if (sscanf(hex.substr(i, 2).c_str(), "%02X", &byte) != 1)
            return "";
        result += static_cast<char>(byte);
    }
    return result;
}

const char* encrypt(const char* text, const char* key) {
    if (!text || !key) {
        fprintf(stderr, "[xor] Ошибка: Один из аргументов равен null\n");
        return nullptr;
    }

    size_t textLen = strlen(text);
    size_t keyLen = strlen(key);

    if (keyLen == 0) {
        fprintf(stderr, "[xor] Ошибка: Ключ пуст\n");
        return nullptr;
    }

    string result;
    result.resize(textLen);

    for (size_t i = 0; i < textLen; ++i) {
        result[i] = text[i] ^ key[i % keyLen];
    }

    string hexResult = to_hex(result);
    return strdup(hexResult.c_str());
}

const char* decrypt(const char* hexText, const char* key) {
    if (!hexText || !key) {
        fprintf(stderr, "[xor] Ошибка: Один из аргументов равен null\n");
        return nullptr;
    }

    string binary = fromHex(hexText);
    if (binary.empty()) {
        fprintf(stderr, "[xor] Ошибка: Невалидный HEX вход\n");
        return nullptr;
    }

    size_t keyLen = strlen(key);
    if (keyLen == 0) {
        fprintf(stderr, "[xor] Ошибка: Ключ пуст\n");
        return nullptr;
    }

    string result;
    result.resize(binary.size());

    for (size_t i = 0; i < binary.size(); ++i) {
        result[i] = binary[i] ^ key[i % keyLen];
    }

    return strdup(result.c_str());
}

    const char* get_name() {
        return "Xor шифрование";
    }

    const char* get_description() {
        return R"(XOR — симметричный побитовый шифр, основанный на логической операции исключающего ИЛИ.
    Каждый байт текста XOR'ится с байтом ключа. Если ключ короче текста, он повторяется по кругу.
    
    Результат шифрования в этой программе возвращается в виде HEX-строки.
    
    Пример:
    Текст:  HELLO
    Ключ:   KEY
    XOR (в HEX): 03 00 4D CA 3D
    
    Тот же ключ используется для дешифрования — повторный XOR возвращает исходный текст.
    
    Ключ: строка (ASCII или UTF-8).)";
    }
    
}