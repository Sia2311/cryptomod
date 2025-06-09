#include "utils.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

std::string readBinaryFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Ошибка чтения бинарного файла: " + filename);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool writeBinaryFile(const std::string& filename, const std::string& data)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Ошибка записи бинарного файла: " + filename);
    }
    file.write(data.data(), static_cast<std::streamsize>(data.size()));
    return true;
}

std::string readFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }
    std::stringstream copy;
    copy << file.rdbuf();
    return copy.str();
}

bool writeFile(const std::string& filename, const std::string& text)
{
    std::ofstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }
    file << text;
    return true;
}

// Преобразование байтов в хекс строку
std::string bytesToHex(const std::string& bytes)
{
    std::ostringstream oss;
    for (unsigned char byte : bytes)
    {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return oss.str();
}

// Преобразование хекс строки в байты
std::string hexToBytes(const std::string& hex)
{
    std::string out;
    for (int i = 0; i < hex.length(); i += 2)
    {
        std::string byteStr = hex.substr(i, 2);
        char byte = static_cast<char>(strtol(byteStr.c_str(), nullptr, 16));
        out.push_back(byte);
    }
    return out;
}

void userPause()
{
    std::cout << "\nНажмите Enter для возврата...";
    std::cin.ignore();
    std::cin.get();
}

void clearCin()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
