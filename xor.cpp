#include "CipherInterface.h"
#include <gmp.h>
#include <string>
#include <cstring>
#include <stdexcept>

using namespace std;

extern "C"{

    const char* encrypt (const char* text, const char* key){

        static string result;
        result.clear();

        if(!text || !key){
            throw invalid_argument("Один из аргументов null");
        }
        //считаем длины строк
        size_t textLen = strlen(text);
        size_t keyLen = strlen(key);

        if(keyLen == 0){
            throw invalid_argument("Ваш ключ пуст!");
        }

        result.resize(textLen);
        for(size_t i = 0; i < textLen; ++i){
            result[i] = text[i] ^ key[i % keyLen];
        }
        return result.c_str();
}
    const char* get_name(){
        return "Xor шифрование";
    }
    const char* get_description(){
        return "Побайтовое шифрование с ключом.";
    }
}
