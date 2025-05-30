#include "CipherInterface.h"
#include <string>
#include <cstring>
#include <stdexcept>
#include <cstdlib>    

using namespace std;

extern "C" {

    const char* encrypt(const char* text, const char* key) {
        if (!text || !key) {
            throw invalid_argument("Один из аргументов равен null");
        }

        size_t textLen = strlen(text);
        size_t keyLen = strlen(key);

        if (keyLen == 0) {
            throw invalid_argument("Ваш ключ пуст!");
        }

        string result;
        result.resize(textLen);

        for (size_t i = 0; i < textLen; ++i) {
            result[i] = text[i] ^ key[i % keyLen];
        }

        return strdup(result.c_str());  //возвращаем копию в куче
    }

    const char* decrypt(const char* text, const char* key) {
        return encrypt(text, key);
    }

    const char* get_name() {
        return "Xor шифрование";
    }

    const char* get_description() {
        return "Побайтовое шифрование с ключом. Симметричное.";
    }
}
