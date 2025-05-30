#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <string>
#include "utils.h"
#include "module_load.h"

using namespace std;

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
            cout << plugins.size() + 1 << ". Выход из программы\n";

            cout << "> ";
            int choice;
            cin >> choice;
            clearCin();

            if (cin.fail()) {
                cout << "Ошибка ввода\n";
                clearCin();
                continue;
            }

            if (choice == static_cast<int>(plugins.size()) + 1) {
                cout << "Выход...\n";
                running = false;
                break;
            }

            if (choice < 1 || choice > static_cast<int>(plugins.size())) {
                cout << "Неверный выбор\n";
                continue;
            }

            CipherPlugin& cipher = plugins[choice - 1];
            cout << "[DEBUG] Выбран шифр: " << cipher.name << "\n";

            // Проверка валидности указателей
            cout << "[DEBUG] Проверка функций:\n";
            cout << "  encrypt: " << (void*)cipher.encrypt << "\n";
            cout << "  decrypt: " << (void*)cipher.decrypt << "\n";
            cout << "  name:    " << cipher.name << "\n";
            cout << "  desc:    " << cipher.description << "\n";

            string inputText;
            cout << "Введите текст: ";
            getline(cin, inputText);
            cout << "[DEBUG] Введённый текст: \"" << inputText << "\"\n";

            if (inputText.empty()) {
                cerr << "Пустой ввод!\n";
                continue;
            }

            cout << "[DEBUG] Вызов encrypt()...\n";
            const char* encrypted = cipher.encrypt(inputText.c_str(), fixedKey);
            cout << "[DEBUG] encrypt() вернул: " << (void*)encrypted << "\n";

            if (!encrypted) {
                cerr << "Ошибка при шифровании (null)\n";
                continue;
            }

            cout << "[DEBUG] Вызов decrypt()...\n";
            const char* decrypted = cipher.decrypt(encrypted, fixedKey);
            cout << "[DEBUG] decrypt() вернул: " << (void*)decrypted << "\n";

            if (!decrypted) {
                cerr << "Ошибка при расшифровке (null)\n";
                free((void*)encrypted);
                continue;
            }

            cout << "\n Зашифрованный текст: ";
            for (int i = 0; encrypted[i] != '\0'; ++i) {
                cout << setw(2) << setfill('0') << hex << uppercase
                     << (int)(unsigned char)encrypted[i] << " ";
            }
            cout << dec << "\n";

            cout << " Расшифрованный текст: " << decrypted << "\n";

            free((void*)encrypted);
            free((void*)decrypted);
            pause();

            unload_plugins(plugins);

        } catch (const exception& ex) {
            cerr << "[EXCEPTION] " << ex.what() << "\n";
            clearCin();
            pause();
        } catch (...) {
            cerr << "[EXCEPTION] Неизвестная ошибка\n";
            clearCin();
            pause();
        }
    }

    cout << "[DEBUG] Программа завершена.\n";
    return 0;
}
