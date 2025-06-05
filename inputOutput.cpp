#include "inputOutput.h"
#include "utils.h"
#include "generator.h"
#include <iostream>
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

InputData getUserInput(const CipherPlugin& cipher, const string& defaultKey) {
    InputData data;

    // Выбор режима: шифровать или дешифровать
    while (true) {
        cout << "Режим:\n1. Шифровать\n2. Дешифровать\n0. Назад\n> ";
        int modeChoice;
        cin >> modeChoice;
        clearCin();

        if (modeChoice == 0) return {};
        if (modeChoice == 1) {
            data.encrypt = true;
            break;
        } else if (modeChoice == 2) {
            data.encrypt = false;
            break;
        } else {
            cout << "Неверный выбор. Повторите ввод.\n";
        }
    }

    // Ввод текста
    while (true) {
        cout << "Ввод:\n1. Ввести вручную\n2. Загрузить из файла\n0. Назад\n> ";
        int inputMode;
        cin >> inputMode;
        clearCin();

        if (inputMode == 0) return {};

        if (inputMode == 1) {
            cout << "Введите текст: ";
            getline(cin, data.text);
            if (data.text.empty()) {
                cout << "Текст не может быть пустым. Повторите.\n";
                continue;
            }
            break;

        } else if (inputMode == 2) {
            cout << "Имя файла: ";
            string filename;
            getline(cin, filename);

            cout << "Тип файла:\n1. Текстовый\n2. Бинарный\n0. Назад\n> ";
            int fileType;
            cin >> fileType;
            clearCin();

            if (fileType == 0) continue;

            if (fileType == 1) {
                data.text = readFile(filename);
                if (data.text.empty()) {
                    cout << "Файл пустой или не удалось прочитать. Повторите.\n";
                    continue;
                }
            } else if (fileType == 2) {
                string raw = readBinaryFile(filename);
                if (raw.empty()) {
                    cout << "Файл пустой или не удалось прочитать. Повторите.\n";
                    continue;
                }
                data.text = bytesToHex(raw);
            } else {
                cout << "Неверный тип. Повторите.\n";
                continue;
            }
            break;

        } else {
            cout << "Неверный режим ввода. Повторите.\n";
        }
    }

    // Ключ
    cout << "Введите ключ (Enter — автогенерация, 0 — назад): ";
    getline(cin, data.key);
    if (data.key == "0") return {};

    if (data.key.empty()) {
        data.key = autoGenerateKey(cipher.name, data.text.length());
        cout << "[Автоключ]: " << data.key << endl;
    }

    return data;
}

bool outputResult(const string& result, const CipherPlugin& cipher, bool encrypt) {
    while (true) {
        cout << "Вывод:\n1. На экран\n2. В файл\n0. Назад\n> ";
        int outputChoice;
        cin >> outputChoice;
        clearCin();

        if (outputChoice == 0) return false;

        if (outputChoice == 1) {
            cout << (encrypt ? "Результат: " : "Расшифрованный текст: ") << result << "\n";
            pause();
            return true;

        } else if (outputChoice == 2) {
            cout << "Имя выходного файла: ";
            string filename;
            getline(cin, filename);

            cout << "Тип файла:\n1. Текстовый \n2. Бинарный\n0. Назад\n> ";
            int fileType;
            cin >> fileType;
            clearCin();

            if (fileType == 0) continue;


            bool ok = false;

            if (fileType == 1) {
                ok = writeFile(filename, result);
            } else if (fileType == 2) {
                string raw = hexToBytes(result);
                ok = writeBinaryFile(filename, raw);
            } else {
                cout << "Неверный тип. Повторите.\n";
                continue;
            }

            if (!ok) {
                cout << "Ошибка записи. Повторите.\n";
                continue;
            }

            cout << "Результат успешно сохранён.\n";
            pause();
            return true;

        } else {
            cout << "Неверный выбор. Повторите.\n";
        }
    }
}
