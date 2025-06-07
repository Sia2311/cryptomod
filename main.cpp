#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <unistd.h>

#include "utils.h"
#include "module_load.h"
#include "inputOutput.h"
#include "verefication.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    //верефикация
    string keyVereficaton;
    cout << "Введите путь к изображению-ключу для доступа: ";
    getline(cin, keyVereficaton);

    if(!verifyImageKey(keyVereficaton) || !checkUSB()){
        cout << "Отлично!\n";
        sleep(3);
        cerr << "ДО СВИДАНИЯ!!!\n";
        sleep(3);
        return 1;
    }
    cout << "Ключ принят. Запуск...\n";
    sleep(1);

    try {
        while (true) {
            auto plugins = loadPlugins("./plugins");

            cout << "\n--------- Главное меню --------\n";
            for (size_t i = 0; i < plugins.size(); ++i) {
                cout << i + 1 << ". " << plugins[i].name << "\n";
            }
            cout << plugins.size() + 1 << ". Выход\n";

            int choice = -1;
            while (true) {
                cout << "> ";
                cin >> choice;
                clearCin();

                if (choice == static_cast<int>(plugins.size()) + 1) {
                    cout << "Выход...\n";
                    unload_plugins(plugins);
                    sleep(2);
                    return 0;
                }

                if (choice >= 1 && choice <= static_cast<int>(plugins.size())) {
                    break; // корректный выбор
                }

                cerr << "Неверный выбор. Пожалуйста, введите номер из списка.\n";
            }

            CipherPlugin& cipher = plugins[choice - 1];
            cout << "\n----------------- " << cipher.name << " ----------------\n";
            cout << "Описание:\n" << cipher.description << "\n";

            bool inCipherMenu = true;
            while (inCipherMenu) {
                try {
                    InputData input = getUserInput(cipher);
                    if (input.text.empty()) {
                        cout << "Отмена операции. Возврат в главное меню...\n";
                        break;
                    }

                    const char* rawResult = input.encrypt
                        ? cipher.encrypt(input.text.c_str(), input.key.c_str())
                        : cipher.decrypt(input.text.c_str(), input.key.c_str());

                    if (!rawResult) {
                        cerr << "Ошибка шифрования/дешифрования.\n";
                        userPause();
                        continue;
                    }

                    string resultStr(rawResult);
                    free((void*)rawResult);

                    if (!outputResult(resultStr, cipher, input.encrypt)) {
                        cout << "Вывод отменён. Возврат в главное меню.\n";
                        break;
                    }

                    inCipherMenu = false;
                } catch (const exception& ex) {
                    cerr << "[Ошибка]: " << ex.what() << "\nПопробуйте снова.\n";
                    userPause();
                }
            }

            unload_plugins(plugins);
        }
    } catch (const exception& ex) {
        cerr << "[Критическая ошибка]: " << ex.what() << "\n";
        userPause();
    } catch (...) {
        cerr << "[Неизвестная ошибка]\n";
        userPause();
    }

    cout << "Завершение работы...\n";
    return 0;
}
