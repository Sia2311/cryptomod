#include "CipherInterface.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

// преобразование в хекс
std::string toHex(const std::string& message)
{
    std::string output;
    for (unsigned char byteValue : message)
    {
        int highNibble = byteValue / 16;
        int lowNibble = byteValue % 16;

        output += static_cast<char>((highNibble < 10) ? ('0' + highNibble) : ('A' + highNibble - 10));
        output += static_cast<char>((lowNibble < 10) ? ('0' + lowNibble) : ('A' + lowNibble - 10));
    }
    return output;
}

// преобразование хекс в байты
std::string hexToBytes(const std::string& hex)
{
    std::string out;
    for (int i = 0; i < hex.length(); i += 2)
    { // по два символа
        std::string byteStr = hex.substr(i, 2);
        char byte = static_cast<char>(strtol(byteStr.c_str(), nullptr, 16));
        out.push_back(byte);
    }
    return out;
}

extern "C"
{
    const char* encrypt(const char* text, const char* key)
    {
        uint64_t textLen = strlen(text);
        uint64_t keyLen = strlen(key);

        std::string result;
        result.resize(textLen);

        for (uint64_t i = 0; i < textLen; ++i)
        {
            result[i] = static_cast<char>(text[i] ^ key[i % keyLen]);
        }

        std::string hexResult = toHex(result);
        return strdup(hexResult.c_str());
    }

    const char* decrypt(const char* hexText, const char* key)
    {
        std::string binary = hexToBytes(hexText);

        uint64_t keyLen = strlen(key);

        std::string result;
        result.resize(binary.size());

        for (uint64_t i = 0; i < binary.size(); ++i)
        {
            result[i] = static_cast<char>(binary[i] ^ key[i % keyLen]);
        }

        return strdup(result.c_str());
    }

    const char* getName()
    {
        return "Xor шифрование";
    }

    const char* getDescription()
    {
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
