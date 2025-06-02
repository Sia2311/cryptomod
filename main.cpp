#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include "utils.h"
#include "module_load.h"

using namespace std;

// Конвертация строки в HEX-представление
string stringToHex(const string& input) {
    ostringstream oss;
    for (unsigned char c : input) {
        oss << setw(2) << setfill('0') << hex << uppercase << static_cast<int>(c);
    }
    return oss.str();
}

// Обратная конвертация HEX в строку байтов
string hexToString(const string& hex) {
    string output;
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        char byte = static_cast<char>(strtol(byteString.c_str(), nullptr, 16));
        output.push_back(byte);
    }
    return output;
}

// Чтение бинарного файла
string readBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Ошибка чтения бинарного файла: " << filename << endl;
        return "";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Запись бинарного файла
bool writeBinaryFile(const string& filename, const string& data) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Ошибка записи бинарного файла: " << filename << endl;
        return false;
    }
    file.write(data.data(), data.size());
    return true;
}

int main() {
    setlocale(LC_ALL, "RU");

    const char* fixedKey = "DEMO_KEY";
    bool running = true;

    while (running) {
        auto plugins = loadPlugins("./plugins");
        try {
            cout << "\n--------- Главное меню --------\n";
            for (size_t i = 0; i < plugins.size(); ++i) {
                cout << i + 1 << ". " << plugins[i].name << "\n";
            }
            cout << plugins.size() + 1 << ". Выход\n> ";

            int choice;
            cin >> choice;
            clearCin();

            if (choice == static_cast<int>(plugins.size()) + 1) {
                cout << "Выход...\n";
                break;
            }

            if (choice < 1 || choice > static_cast<int>(plugins.size())) {
                cerr << "Неверный выбор\n";
                continue;
            }

            CipherPlugin& cipher = plugins[choice - 1];

            cout << "Режим:\n1. Шифровать\n2. Дешифровать\n> ";
            int modeChoice;
            cin >> modeChoice;
            clearCin();

            bool isEncrypt = (modeChoice == 1);
            if (!isEncrypt && modeChoice != 2) {
                cerr << "Неверный режим\n";
                continue;
            }

            // Ввод текста
            string inputText;
            cout << "Ввод:\n1. Ввести вручную\n2. Загрузить из файла\n> ";
            int inputMode;
            cin >> inputMode;
            clearCin();

            if (inputMode == 1) {
                cout << "Введите текст: ";
                getline(cin, inputText);
            } else if (inputMode == 2) {
                cout << "Имя файла: ";
                string filename;
                getline(cin, filename);

                cout << "Тип файла:\n1. Текстовый (hex)\n2. Бинарный\n> ";
                int fileType;
                cin >> fileType;
                clearCin();


                if (fileType == 1) {
                    string fileContent = readFile(filename);
                    if (fileContent.empty()) {
                        cerr << "Ошибка чтения или пустой файл.\n";
                        continue;
                    }
                    // Преобразуем в байты ТОЛЬКО если шифр не ожидает уже hex
                    if (!cipher.returnHex && !isEncrypt) {
                        inputText = hexToString(fileContent);
                    } else {
                        inputText = fileContent;
                    }
                } else if (fileType == 2) {
                    inputText = readBinaryFile(filename);
                } else {
                    cerr << "Неверный тип файла.\n";
                    continue;
                }
            } else {
                cerr << "Неверный режим ввода.\n";
                continue;
            }

            if (inputText.empty()) {
                cerr << "Пустой ввод\n";
                continue;
            }

            // Ключ
            string userKey;
            cout << "Введите ключ (Enter — по умолчанию): ";
            getline(cin, userKey);
            if (userKey.empty()) userKey = fixedKey;

            // Выполнение шифрования/дешифрования
            const char* result = isEncrypt
                ? cipher.encrypt(inputText.c_str(), userKey.c_str())
                : cipher.decrypt(inputText.c_str(), userKey.c_str());

            if (!result) {
                cerr << "Ошибка во время выполнения.\n";
                continue;
            }

            string resultStr(result);

            // Сохранение
            cout << "Вывод:\n1. На экран\n2. В файл\n> ";
            int outputChoice;
            cin >> outputChoice;
            clearCin();

            if (outputChoice == 1) {
                if (isEncrypt) {
                    if (cipher.returnHex) {
                        cout << "Результат: " << resultStr << "\n";
                    } else {
                        cout << "Результат: " << stringToHex(resultStr) << "\n";
                    }
                } else {
                    cout << "Расшифрованный текст: " << resultStr << "\n";
                }
            } else if (outputChoice == 2) {
                cout << "Имя выходного файла: ";
                string outFile;
                getline(cin, outFile);

                cout << "Тип файла:\n1. Текстовый (hex)\n2. Бинарный\n> ";
                int fileType;
                cin >> fileType;
                clearCin();

                bool ok = false;

                if (fileType == 1) {
                    if (isEncrypt) {
                        ok = writeFile(outFile, cipher.returnHex ? resultStr : stringToHex(resultStr));
                    } else {
                        ok = writeFile(outFile, resultStr); // результат — обычный текст
                    }
                } else if (fileType == 2) {
                    ok = writeBinaryFile(outFile, resultStr);
                } else {
                    cerr << "Неверный тип файла.\n";
                    free((void*)result);
                    continue;
                }

                if (ok) {
                    cout << "Результат успешно сохранён.\n";
                } else {
                    cerr << "Ошибка при сохранении.\n";
                }
            } else {
                cerr << "Неверный режим вывода.\n";
            }

            free((void*)result);
            pause();
            unload_plugins(plugins);

        } catch (const exception& ex) {
            cerr << "Ошибка: " << ex.what() << "\n";
            clearCin();
            pause();
        } catch (...) {
            cerr << "Неизвестная ошибка\n";
            clearCin();
            pause();
        }
    }

    cout << "[DEBUG] Завершение работы.\n";
    return 0;
}
