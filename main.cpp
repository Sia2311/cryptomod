#include <iostream>
#include <vector>
#include <limits>
#include "utils.h"
#include "module_load.h"
#include "cipherSession.h"

using namespace std;

enum class MainMenu {
    SELECT_CIPHER = 1,
    EXIT
};

int main() {
    setlocale(LC_ALL, "RU");
    bool running = true;

    while (running) {
        try {
            int code;
            bool ValidInput = false;
            bool firsttime = true;

            while (!ValidInput) {
                if (firsttime) {
                    cout << "\n--------- Главное меню --------\n";
                    cout << "Выберите действие:\n";
                    cout << "1. Выбрать и использовать шифр\n";
                    cout << "2. Выход из программы\n";
                }

                cout << "> ";
                cin >> code;
                clearCin();
                firsttime = false;

                if (cin.fail()) {
                    cout << "Ошибка: Введите число от 1 до 2\n";
                    clearCin();
                    continue;
                }

                auto task = static_cast<MainMenu>(code);

                switch (task) {
                    case MainMenu::SELECT_CIPHER: {
                        auto plugins = loadPlugins("./plugins");

                        if (plugins.empty()) {
                            cout << "Нет доступных шифров.\n";
                            pause();
                            break;
                        }

                        cout << "\nДоступные шифры:\n";
                        for (size_t i = 0; i < plugins.size(); ++i) {
                            cout << i + 1 << ". " << plugins[i].name << "\n";
                        }

                        cout << "Выберите шифр: ";
                        int index;
                        cin >> index;
                        clearCin();

                        if (index < 1 || index > static_cast<int>(plugins.size())) {
                            cout << "Неверный номер.\n";
                            pause();
                            break;
                        }

                        run_cipher_session(plugins[index - 1]);

                        unload_plugins(plugins);
                        break;
                    }

                    case MainMenu::EXIT:
                        cout << "Выход из программы...\n";
                        running = false;
                        break;

                    default:
                        cout << "Нет такого действия! Повторите ввод.\n";
                        continue;
                }

                ValidInput = true;
                firsttime = true;

                if (running && task != MainMenu::EXIT) {
                    pause();
                }
            }

        } catch (const exception& ex) {
            cerr << "Ошибка: " << ex.what() << endl;
            clearCin();
            pause();
        } catch (...) {
            cerr << "Неизвестная ошибка\n";
            clearCin();
            pause();
        }
    }

    return 0;
}