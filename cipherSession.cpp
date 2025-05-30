#include "cipherSession.h"
#include "module_load.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <limits>
#include <iomanip> 

using namespace std;

enum class CipherAction {
    ENCRYPT_AND_DECRYPT = 1,
    DESCRIPTION,
    BACK
};

void run_cipher_session(CipherPLugin& cipher) {
    bool inSession = true;
    const char* fixedKey = "DEMO_KEY"; 

    while (inSession) {
        try {
            int code;
            bool ValidInput = false;
            bool firsttime = true;

            while (!ValidInput) {
                if (firsttime) {
                    cout << "\n--- Работа с шифром: " << cipher.name << " ---\n";
                    cout << "1. Зашифровать и расшифровать\n";
                    cout << "2. Показать описание\n";
                    cout << "3. Назад\n";
                }

                cout << "> ";
                cin >> code;
                clearCin();
                firsttime = false;

                if (cin.fail()) {
                    cout << "Ошибка: Введите число от 1 до 3\n";
                    clearCin();
                    continue;
                }

                auto action = static_cast<CipherAction>(code);

                switch (action) {
                    case CipherAction::ENCRYPT_AND_DECRYPT: {
                        string inputText;
                        cout << "Введите текст: ";
                        getline(cin, inputText);

                        try {
                            const char* encrypted = cipher.encrypt(inputText.c_str(), fixedKey);
                            const char* decrypted = cipher.decrypt(encrypted, fixedKey);

                            cout << "\n Зашифрованный текст: ";
                            for (int i = 0; encrypted[i] != '\0'; ++i) {
                                cout << setw(2) << setfill('0') << hex << uppercase << (int)(unsigned char)encrypted[i] << " ";
                            }
                            cout << dec << endl;

                            cout << " Расшифрованный текст: " << decrypted << "\n";

                            // Освобождение памяти
                            free((void*)encrypted);
                            free((void*)decrypted);

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
