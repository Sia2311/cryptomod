#include "inputOutput.h"
#include "utils.h"
#include <iostream>
#include <stdexcept>

using namespace std;

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

        cout << "Тип файла:\n1. Текстовый \n2. Бинарный\n> ";
        int fileType;
        cin >> fileType;
        clearCin();

        if (fileType == 1) {
            string content = readFile(filename);
            if (content.empty()) throw runtime_error("Файл пустой или не удалось прочитать: " + filename);

            if (!cipher.returnHex && !data.encrypt) {
                data.text = hexToString(content);
            } else {
                data.text = content;
            }

        } else if (fileType == 2) {
            data.text = readBinaryFile(filename);
            if (data.text.empty()) throw runtime_error("Файл пустой или не удалось прочитать: " + filename);
        } else {
            throw runtime_error("Неверный тип файла: ожидалось 1 или 2.");
        }

    } else {
        throw runtime_error("Неверный режим ввода: ожидалось 1 или 2.");
    }

    cout << "Введите ключ (Enter — по умолчанию): ";
    getline(cin, data.key);
    if (data.key.empty()) data.key = defaultKey;

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
            cout << "Результат: " 
                 << (cipher.returnHex ? result : stringToHex(result)) << "\n";
        } else {
            cout << "Расшифрованный текст: " << result << "\n";
        }
        return true;

    } else if (outputChoice == 2) {
        cout << "Имя выходного файла: ";
        string filename;
        getline(cin, filename);

        cout << "Тип файла:\n1. Текстовый (hex)\n2. Бинарный\n> ";
        int fileType;
        cin >> fileType;
        clearCin();

        bool ok = false;

        if (fileType == 1) {
            string outputData = encrypt && !cipher.returnHex ? stringToHex(result) : result;
            ok = writeFile(filename, outputData);
        } else if (fileType == 2) {
            ok = writeBinaryFile(filename, result);
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
