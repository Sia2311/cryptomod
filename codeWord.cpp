#include <cstdint>
#include <cstring>
#include <set>
#include <string>
#include <vector>

#include "CipherInterface.h"

// строим новый алфавит по ключу
std::vector<uint8_t> buildingAlphabet(const std::string& key)
{
    std::vector<uint8_t> base(256);
    for (int i = 0; i < 256; ++i)
    {
        base[i] = static_cast<uint8_t>(i);
    }

    std::set<uint8_t> seen;
    std::vector<uint8_t> result;
    // заполняем байта из ключа
    for (char keyChar : key)
    {
        uint8_t keyByte = static_cast<uint8_t>(keyChar);
        if (seen.count(keyByte) == 0U)
        {
            result.push_back(keyByte);
            seen.insert(keyByte);
        }
    }
    // остальные байты алфавита
    for (uint8_t keyByte : base)
    {
        if (seen.count(keyByte) == 0U)
        {
            result.push_back(keyByte);
        }
    }

    return result;
}

// Перевод строки в HEX
std::string toHex(const std::string& message)
{
    std::string output;
    for (unsigned char currentChar : message)
    {
        int high = currentChar / 16;
        int low = currentChar % 16;

        char first = static_cast<char>((high < 10) ? ('0' + high) : ('A' + high - 10));
        char second = static_cast<char>((low < 10) ? ('0' + low) : ('A' + low - 10));

        output += first;
        output += second;
    }
    return output;
}

// Преобразование хекса обратно в байты
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
        if ((text == nullptr) || (key == nullptr))
        {
            return nullptr;
        }
        // строим алфавит шифрования
        std::vector<uint8_t> newAlphabet = buildingAlphabet(key);
        uint8_t encryptMap[256];
        for (int i = 0; i < 256; ++i)
        {
            encryptMap[i] = newAlphabet[i];
        }
        // заменяем по алфваиту
        std::string input(text);
        std::string encrypted;
        encrypted.reserve(input.size());

        for (unsigned char inputChar : input)
        {
            encrypted.push_back(static_cast<char>(encryptMap[static_cast<uint8_t>(inputChar)]));
        }

        std::string hex = toHex(encrypted);
        return strdup(hex.c_str());
    }

    const char* decrypt(const char* hexText, const char* key)
    {
        if ((hexText == nullptr) || (key == nullptr))
        {
            return nullptr;
        }

        std::string byte = hexToBytes(hexText);
        if (byte.empty())
        {
            return nullptr;
        }
        // строим обратную табличку
        std::vector<uint8_t> alphabet = buildingAlphabet(key);
        uint8_t decryptMap[256];
        for (int i = 0; i < 256; ++i)
        {
            decryptMap[alphabet[i]] = static_cast<uint8_t>(i);
        }
        // дешифруем
        std::string result;
        result.reserve(byte.size());

        for (unsigned char inputChar : byte)
        {
            result.push_back(static_cast<char>(decryptMap[static_cast<uint8_t>(inputChar)]));
        }

        return strdup(result.c_str());
    }

    const char* getName()
    {
        return "Шифр кодового слова";
    }

    const char* getDescription()
    {
        return R"(Шифр кодового слова — байтовая перестановка, основанная на введённом ключе.
На его основе строится новый алфавит: сначала добавляются уникальные символы ключа в порядке появления,
затем оставшиеся байты (0–255), не входящие в ключ.

Каждый символ исходного текста заменяется на байт из нового алфавита с тем же индексом.

Пример:
Ключ:  ANASTASIA BORISOVNA
Первые байты нового алфавита:  'A', 'N', 'A', 'S', 'T', 'A', ...

Текст:  AVTF
Результат (в HEX): 38 55 50 3D

Для расшифровки используется тот же ключ — алфавит восстанавливается в исходном порядке.

Ключ: строка (ASCII или UTF-8). Регистр имеет значение. Результат шифрования выводится в виде HEX.)";
    }
}
