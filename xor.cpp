#include "CipherInterface.h"
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>  

using namespace std;

extern "C" {

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

        return strdup(result.c_str());  // выделяем копию строки в куче
    }

    const char* decrypt(const char* text, const char* key) {
        // XOR шифрование симметрично — используем ту же функцию
        return encrypt(text, key);
    }

    const char* get_name() {
        return "Xor шифрование";
    }

    const char* get_description() {
        return "Побайтовое шифрование с ключом. Симметричное.";
    }
}
