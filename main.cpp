#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "utils.h"
#include "module_load.h"
#include "inputOutput.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");
    const string fixedKey = "DEMO_KEY";
    bool running = true;

    try {
        while (running) {
            auto plugins = loadPlugins("./plugins");

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
                pause();
                continue;
            }

            CipherPlugin& cipher = plugins[choice - 1];
            cout << "\n-----------------" << cipher.name << "---------------\n";
            cout << "Описание:\n " << cipher.description << endl;;

            // Получение ввода
            InputData input = getUserInput(cipher, fixedKey);

            // Шифрование или дешифровка
            const char* rawResult = input.encrypt
                ? cipher.encrypt(input.text.c_str(), input.key.c_str())
                : cipher.decrypt(input.text.c_str(), input.key.c_str());

            if (!rawResult) {
                throw runtime_error("Ошибка выполнения шифрования/дешифрования.");
            }

            string resultStr(rawResult);
            free((void*)rawResult);  // освобождение памяти от плагина

            // Вывод результата
            outputResult(resultStr, cipher, input.encrypt);

            pause();
            unload_plugins(plugins);
        }
    } catch (const exception& ex) {
        cerr << "[Ошибка]: " << ex.what() << "\n";
        pause();
    } catch (...) {
        cerr << "[Неизвестная ошибка]\n";
        pause();
    }

    cout << "[DEBUG] Завершение работы.\n";
    return 0;
}