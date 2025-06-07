#include "inputOutput.h"
#include "utils.h"
#include "generator.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

// Преобразование байтов в хекс строку
string bytesToHex(const string& bytes) {
    ostringstream oss;
    for (unsigned char byte : bytes) {
        oss << hex << setw(2) << setfill('0') << static_cast<int>(byte);
    }
    return oss.str();
}

// Преобразование хекс строки в байты
string hexToBytes(const string& hex) {
    string out;
    for (int i = 0; i < hex.length(); i += 2) {
        string byteStr = hex.substr(i, 2);
        char byte = static_cast<char>(strtol(byteStr.c_str(), nullptr, 16));
        out.push_back(byte);
    }
    return out;
}

InputData getUserInput(const CipherPlugin& cipher) {
    while (true) {
        try {
            InputData data;

            // Выбор режима
            while (true) {
                int modeChoice;
                cout << "Режим:\n1. Шифровать\n2. Дешифровать\n0. Назад\n> ";
                
                if (!(cin >> modeChoice)) {
                    clearCin(); 
                    cout << "Некорректный ввод. Пожалуйста, введите 0, 1 или 2.\n";
                    continue; 
                }
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

                int inputMode;
                cout << "Ввод:\n1. Ввести вручную\n2. Загрузить из файла\n0. Назад\n> ";
                if(!(cin >> inputMode)){
                clearCin();
                cout << "Некорректный ввод. Пожалуйста, введите 0, 1 или 2.\n";
                continue;
                }
                clearCin();

                if (inputMode == 0) return {};

                if (inputMode == 1) {
                    cout << "Введите текст: ";
                    if (!getline(cin, data.text)) throw runtime_error("Ошибка чтения ввода.");
                    if (data.text.empty()) throw runtime_error("Текст не может быть пустым.");
                    break;

                } else if (inputMode == 2) {
                    cout << "Путь и имя файла: ";
                    string filename;
                    getline(cin, filename);

                    cout << "Тип файла:\n1. Текстовый\n2. Бинарный\n0. Назад\n> ";
                    int fileType;
                    cin >> fileType;
                    clearCin();

                    if (fileType == 0) continue;

                try{
                    if (fileType == 1) {
                        data.text = readFile(filename);
                        
                    } else if (fileType == 2) {
                        string raw = readBinaryFile(filename);
         
                        data.text = bytesToHex(raw);
                    } else {
                        cout << "Неверный тип. Повторите.\n";
                        continue;
                    }
                    if (data.text.empty()){
                        throw runtime_error("Файл пуст.");
                    }
                    break;
                } catch (const exception& e){
                    cerr << "[Ошибка файла]: " << e.what() << "\n";
                }

                } else {
                    cout << "Неверный выбор! Повторите!\n";
                }
            }

            if (data.encrypt) {
                cout << "Введите ключ (Enter — автогенерация, 0 — назад): ";
            } else {
                cout << "Введите ключ (0 — назад): ";
            }
            getline(cin, data.key);
            
            if (data.key == "0") return {};
            
            // генерация только при шифре
            if (data.key.empty()) {
                if (data.encrypt) {
                    data.key = autoGenerateKey(cipher.name, data.text.length());
                    cout << "[Автоключ]: " << data.key << endl;
                } else {
                    throw runtime_error("Ключ обязателен при дешифровке.");
                }
            }
            
            

            return data;

        } catch (const exception& e) {
            cerr << "[Ошибка ввода]: " << e.what() << "\nПопробуйте снова.\n";
            userPause();
        }
    }
}

bool outputResult(const string& result, const CipherPlugin& cipher, bool encrypt) {
    while (true) {
        try {
            cout << "Вывод:\n1. На экран\n2. В файл\n0. Назад\n> ";
            int outputChoice;
            cin >> outputChoice;
            clearCin();

            if (outputChoice == 0) return false;

            if (outputChoice == 1) {
                cout << (encrypt ? "Результат: " : "Расшифрованный текст: ") << result << "\n";
                userPause();
                return true;

            } else if (outputChoice == 2) {
                cout << "Путь и имя выходного файла: ";
                string filename;
                getline(cin, filename);

                cout << "Тип файла:\n1. Текстовый \n2. Бинарный\n0. Назад\n> ";
                int fileType;
                cin >> fileType;
                clearCin();

                if (fileType == 0) continue;

                bool ok = false;

            try{
                if (fileType == 1) {
                    writeFile(filename, result);
                } else if (fileType == 2) {
                    string raw = hexToBytes(result);
                    writeBinaryFile(filename, raw);
                } else {
                    cout << "Неверный тип. Повторите.\n";
                    continue;
                }

                cout << "Результат успешно сохранён.\n";
                userPause();
                return true;

            }catch (const exception&e){
                cerr << "[Ошибка вывода]: " << e.what() << "\n";
                userPause();
                continue;;
            }

            } else {
                cout << "Неверный выбор. Повторите.\n";
            }

        } catch (const exception& e) {
            cerr << "[Ошибка вывода]: " << e.what() << "\nПопробуйте снова.\n";
            userPause();
        }
    }
}
