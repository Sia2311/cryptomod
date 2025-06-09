#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "CipherInterface.h"

// делим строку на отдельные ютф симмволы
std::vector<std::string> utfSplit(const std::string& input)
{
    std::vector<std::string> result;
    uint64_t byteIndex = 0;
    while (byteIndex < input.size())
    {
        unsigned char leatByte = input[byteIndex];
        int len = 1;

        if ((leatByte & 0x80) == 0x00)
        {
            len = 1; // обычный аски
        }
        else if ((leatByte & 0xE0) == 0xC0)
        {
            len = 2; // 2 байта
        }
        else if ((leatByte & 0xF0) == 0xE0)
        {
            len = 3; // 3 байта
        }
        else if ((leatByte & 0xF8) == 0xF0)
        {
            len = 4; // 4 байта
        }

        result.push_back(input.substr(byteIndex, len));
        byteIndex += len;
    }
    return result;
}

std::string encryptSkitale(const std::string& text, int columns)
{
    if (columns < 2)
    {
        throw std::invalid_argument("[Скитала] Ключ должен быть положительным числом и больше еденицы!");
    }
    if (text.empty())
    {
        return "";
    }
    // строим табличку
    std::vector<std::string> letters = utfSplit(text);
    int rows = static_cast<int>(ceil(static_cast<double>(letters.size()) / columns)); // кол-во строк
    std::vector<std::vector<std::string>> grid(rows, std::vector<std::string>(columns, " "));
    // заполняем
    int index = 0;
    for (int rowIndex = 0; rowIndex < rows && index < letters.size(); ++rowIndex)
    {
        for (int columnIndex = 0; columnIndex < columns && index < letters.size(); ++columnIndex)
        {
            grid[rowIndex][columnIndex] = letters[index++];
        }
    }
    // читаем по столбцам
    std::string result;
    for (int columnIndex = 0; columnIndex < columns; ++columnIndex)
    {
        for (int rowIndex = 0; rowIndex < rows; ++rowIndex)
        {
            result += grid[rowIndex][columnIndex];
        }
    }
    return result;
}

std::string decryptSkitale(const std::string& text, int columns)
{
    if (columns < 2)
    {
        throw std::invalid_argument("[Скитала] Ключ должен быть положительным числом и больше еденицы!");
    }
    if (text.empty())
    {
        return "";
    }
    // создаем
    std::vector<std::string> letters = utfSplit(text);
    int rows = static_cast<int>(ceil(static_cast<double>(letters.size()) / columns));
    int totalCells = rows * columns;
    int pad = totalCells - static_cast<int>(letters.size()); // сколько пустых

    std::vector<std::vector<std::string>> grid(rows, std::vector<std::string>(columns, " "));
    // заполняем по столбцам
    int index = 0;
    for (int columnIndex = 0; columnIndex < columns && index < letters.size(); ++columnIndex)
    {
        for (int rowIndex = 0; rowIndex < rows && index < letters.size(); ++rowIndex)
        {
            if (columnIndex >= columns - pad && rowIndex == rows - 1)
            {
                continue; // пусто
            }
            grid[rowIndex][columnIndex] = letters[index++];
        }
    }
    // читаем по строкам
    std::string result;
    for (int rowIndex = 0; rowIndex < rows; ++rowIndex)
    {
        for (int columnIndex = 0; columnIndex < columns; ++columnIndex)
        {
            result += grid[rowIndex][columnIndex];
        }
    }
    return result;
}
// проверка что ключ число

bool isPositiveInteger(const std::string& str)
{
    if (str.empty())
    {
        return false;
    }
    for (char symbol : str)
    {
        if (isdigit(static_cast<unsigned char>(symbol)) == 0)
        {
            return false;
        }
    }
    return true;
}

extern "C"
{
    const char* encrypt(const char* text, const char* key)
    {
        if ((text == nullptr) || (key == nullptr))
        {
            return nullptr;
        }

        std::string keyStr(key);
        if (!isPositiveInteger(keyStr))
        {
            std::cerr << "[Скитала] Ключ должен быть числом!\n";
            return nullptr;
        }

        int columns = std::stoi(keyStr);

        try
        {
            std::string result = encryptSkitale(text, columns);
            return strdup(result.c_str());
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << e.what() << std::endl;
            return nullptr;
        }
    }

    const char* decrypt(const char* text, const char* key)
    {
        if ((text == nullptr) || (key == nullptr))
        {
            return nullptr;
        }

        std::string keyStr(key);
        if (!isPositiveInteger(keyStr))
        {
            std::cerr << "[Скитала] Ключ должен быть числом!\n";
            return nullptr;
        }

        int columns = std::stoi(keyStr);

        try
        {
            std::string result = decryptSkitale(text, columns);
            return strdup(result.c_str());
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << e.what() << std::endl;
            return nullptr;
        }
    }

    const char* getName()
    {
        return "Скитала (перестановка)";
    }

    const char* getDescription()
    {
        return R"(Скитала — античный шифр перестановки, использовавшийся в Спарте.
Текст записывается по строкам в таблицу с фиксированным числом столбцов (ключ),
затем считывается по столбцам, формируя зашифрованную последовательность.

Пример: при ключе 4 и тексте "ПРИВЕТМИР" таблица будет:

П Р И В
Е Т М И
Р _ _ _

Шифр: ПЕРРТ ИМ ВИ


Ключ: целое число — количество столбцов.)";
    }
}
