#include "cipherSession.h"
#include "module_load.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

enum class CipherAction {
    ENCRYPT = 1,
    DECRYPT,
    DESCRIPTION,
    BACK
};

void run_cipher_session(CipherPLugin& cipher) {
    bool inSession = true;

    while (inSession) {
        try {
            int code;
            bool ValidInput = false;
            bool firsttime = true;

            while (!ValidInput) {
                if (firsttime) {
                    cout << "\n--- Работа с шифром: " << cipher.name << " ---\n";
                    cout << "1. Зашифровать\n";
                    cout << "2. Расшифровать\n";
                    cout << "3. Показать описание\n";
                    cout << "4. Назад\n";
                }

                cout << "> ";
                cin >> code;
                clearCin();
                firsttime = false;

                if (cin.fail()) {
                    cout << "Ошибка: Введите число от 1 до 4\n";
                    clearCin();
                    continue;
                }

                auto action = static_cast<CipherAction>(code);

                switch (action) {
                    case CipherAction::ENCRYPT:
                    case CipherAction::DECRYPT: {
                        string text, key;
                        cout << "Введите текст: ";
                        getline(cin, text);
                        cout << "Введите ключ: ";
                        getline(cin, key);

                        try {
                            const char* result = (action == CipherAction::ENCRYPT)
                                ? cipher.encrypt(text.c_str(), key.c_str())
                                : cipher.decrypt(text.c_str(), key.c_str());

                            cout << "Результат шифрования: ";
                            for (int i = 0; result[i]!= '\0'; ++i){
                                cout << hex << uppercase << (int)(unsigned char)result[i];
                            }
                            cout << dec << endl;
                        } catch (const exception& ex) {
                            cerr << "Ошибка шифра: " << ex.what() << "\n";
                        }
                        pause();
                        break;
                    }

                    case CipherAction::DESCRIPTION:
                        cout << "Описание: " << cipher.description << "\n";
                        pause();
                        break;

                    case CipherAction::BACK:
                        inSession = false;
                        break;

                    default:
                        cout << "Нет такой команды. Повторите ввод.\n";
                        break;
                }

                ValidInput = true;
                firsttime = true;
            }

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
}
