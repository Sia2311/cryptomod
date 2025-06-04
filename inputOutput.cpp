#include "inputOutput.h"
#include "utils.h"
#include "generator.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <iomanip>

using namespace std;

// Преобразование байтов в hex-строку
string bytesToHex(const string& bytes) {
    ostringstream oss;
    for (unsigned char byte : bytes) {
        oss << hex << setw(2) << setfill('0') << static_cast<int>(byte);
    }
    return oss.str();
}

// Преобразование hex-строки в байты
string hexToBytes(const string& hex) {
    string out;
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteStr = hex.substr(i, 2);
        char byte = static_cast<char>(strtol(byteStr.c_str(), nullptr, 16));
        out.push_back(byte);
    }
    return out;
}

InputData getUserInput(const CipherPlugin& cipher, const std::string& defaultKey) {
    InputData data;

    cout << "Режим:\n1. Шифровать\n2. Дешифровать\n> ";
    int modeChoice;
    cin >> modeChoice;
    clearCin();

    if (modeChoice == 1) {
        data.encrypt = true;
    } else if (modeChoice == 2) {
        data.encrypt = false;
    } else {
        throw runtime_error("Неверный режим: ожидалось 1 или 2.");
    }

    cout << "Ввод:\n1. Ввести вручную\n2. Загрузить из файла\n> ";
    int inputMode;
    cin >> inputMode;
    clearCin();

    if (inputMode == 1) {
        cout << "Введите текст: ";
        getline(cin, data.text);
    } else if (inputMode == 2) {
        cout << "Имя файла: ";
        string filename;
        getline(cin, filename);

        cout << "Тип файла:\n1. Текстовый\n2. Бинарный\n> ";
        int fileType;
        cin >> fileType;
        clearCin();

        if (fileType == 1) {
            data.text = readFile(filename);
            if (data.text.empty()) throw runtime_error("Файл пустой или не удалось прочитать: " + filename);
        } else if (fileType == 2) {
            string raw = readBinaryFile(filename);
            if (raw.empty()) throw runtime_error("Файл пустой или не удалось прочитать: " + filename);
            data.text = bytesToHex(raw);  // Преобразуем в hex
        } else {
            throw runtime_error("Неверный тип файла: ожидалось 1 или 2.");
        }
    } else {
        throw runtime_error("Неверный режим ввода: ожидалось 1 или 2.");
    }

    cout << "Введите ключ (Enter — автогенерация): ";
    getline(cin, data.key);

    if (data.key.empty()) {
        data.key = autoGenerateKey(cipher.name, data.text.length());
        cout << "[Автоключ]: " << data.key << endl;
    }

    if (data.text.empty()) {
        throw runtime_error("Пустой ввод текста.");
    }

    return data;
}

bool outputResult(const string& result, const CipherPlugin& cipher, bool encrypt) {
    cout << "Вывод:\n1. На экран\n2. В файл\n> ";
    int outputChoice;
    cin >> outputChoice;
    clearCin();

    if (outputChoice == 1) {
        if (encrypt) {
            cout << "Результат: " << result << "\n";
        } else {
            cout << "Расшифрованный текст: " << result << "\n";
        }
        return true;

    } else if (outputChoice == 2) {
        cout << "Имя выходного файла: ";
        string filename;
        getline(cin, filename);

        cout << "Тип файла:\n1. Текстовый (hex или строка)\n2. Бинарный\n> ";
        int fileType;
        cin >> fileType;
        clearCin();

        bool ok = false;

        if (fileType == 1) {
            ok = writeFile(filename, result);
        } else if (fileType == 2) {
            string raw = hexToBytes(result);  // hex v байты
            ok = writeBinaryFile(filename, raw);
        } else {
            throw runtime_error("Неверный тип выходного файла.");
        }

        if (!ok) throw runtime_error("Ошибка записи в файл: " + filename);
        cout << "Результат успешно сохранён.\n";
        return true;

    } else {
        throw runtime_error("Неверный режим вывода: ожидалось 1 или 2.");
    }
}
