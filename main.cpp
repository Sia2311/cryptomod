#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "inputOutput.h"
#include "module_load.h"
#include "utils.h"
#include "verefication.h"

int main()
{
    setlocale(LC_ALL, "RU");
    // верефикация
    std::string keyVereficaton;
    std::cout << "Введите путь к изображению-ключу для доступа: ";
    std::getline(std::cin, keyVereficaton);

    if (!verifyImageKey(keyVereficaton) || !checkUSB())
    {
        std::cout << "Отлично!\n";
        sleep(3);
        std::cerr << "ДО СВИДАНИЯ!!!\n";
        sleep(3);
        return 1;
    }
    std::cout << "Ключ принят. Запуск...\n";
    sleep(1);

    try
    {
        while (true)
        {
            auto plugins = loadPlugins("./plugins");

            std::cout << "\n--------- Главное меню --------\n";
            for (size_t i = 0; i < plugins.size(); ++i)
            {
                std::cout << i + 1 << ". " << plugins[i].name << "\n";
            }
            std::cout << plugins.size() + 1 << ". Выход\n";

            int choice = -1;
            while (true)
            {
                std::cout << "> ";
                std::cin >> choice;
                clearCin();

                if (choice == static_cast<int>(plugins.size()) + 1)
                {
                    std::cout << "Выход...\n";
                    unloadPlugins(plugins);
                    sleep(2);
                    return 0;
                }

                if (choice >= 1 && choice <= static_cast<int>(plugins.size()))
                {
                    break; // корректный выбор
                }

                std::cerr << "Неверный выбор. Пожалуйста, введите номер из списка.\n";
            }

            CipherPlugin& cipher = plugins[choice - 1];
            std::cout << "\n----------------- " << cipher.name << " ----------------\n";
            std::cout << "Описание:\n" << cipher.description << "\n";

            bool inCipherMenu = true;
            while (inCipherMenu)
            {
                try
                {
                    InputData input = getUserInput(cipher);
                    if (input.text.empty())
                    {
                        std::cout << "Отмена операции. Возврат в главное меню...\n";
                        break;
                    }

                    const char* rawResult = input.encrypt ? cipher.encrypt(input.text.c_str(), input.key.c_str())
                                                          : cipher.decrypt(input.text.c_str(), input.key.c_str());

                    if (rawResult == nullptr)
                    {
                        std::cerr << "Ошибка шифрования/дешифрования.\n";
                        userPause();
                        continue;
                    }

                    std::string resultStr(rawResult);
                    free((void*) rawResult);

                    if (!outputResult(resultStr, input.encrypt))
                    {
                        std::cout << "Вывод отменён. Возврат в главное меню.\n";
                        break;
                    }

                    inCipherMenu = false;
                }
                catch (const std::exception& ex)
                {
                    std::cerr << "[Ошибка]: " << ex.what() << "\nПопробуйте снова.\n";
                    userPause();
                }
            }

            unloadPlugins(plugins);
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "[Критическая ошибка]: " << ex.what() << "\n";
        userPause();
    }
    catch (...)
    {
        std::cerr << "[Неизвестная ошибка]\n";
        userPause();
    }

    std::cout << "Завершение работы...\n";
    return 0;
}
