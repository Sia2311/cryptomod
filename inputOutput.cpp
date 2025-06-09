#include "inputOutput.h"

#include <iostream>

#include "generator.h"
#include "utils.h"

InputData getUserInput(const CipherPlugin& cipher)
{
    while (true)
    {
        try
        {
            InputData data;

            while (true)
            {
                int modeChoice = 0;
                std::cout << "Режим:\n1. Шифровать\n2. Дешифровать\n0. Назад\n> ";

                if (!(std::cin >> modeChoice))
                {
                    clearCin();
                    std::cout << "Некорректный ввод. Пожалуйста, введите 0, 1 или 2.\n";
                    continue;
                }
                clearCin();

                if (modeChoice == 0)
                {
                    return {};
                }
                if (modeChoice == 1)
                {
                    data.encrypt = true;
                    break;
                }
                if (modeChoice == 2)
                {
                    data.encrypt = false;
                    break;
                }
                std::cout << "Неверный выбор. Повторите ввод.\n";
            }

            // Ввод текста
            while (true)
            {
                int inputMode = 0;
                std::cout << "Ввод:\n1. Ввести вручную\n2. Загрузить из файла\n0. Назад\n> ";
                if (!(std::cin >> inputMode))
                {
                    clearCin();
                    std::cout << "Некорректный ввод. Пожалуйста, введите 0, 1 или 2.\n";
                    continue;
                }
                clearCin();

                if (inputMode == 0)
                {
                    return {};
                }

                if (inputMode == 1)
                {
                    std::cout << "Введите текст: ";
                    if (!std::getline(std::cin, data.text))
                    {
                        throw std::runtime_error("Ошибка чтения ввода.");
                    }
                    if (data.text.empty())
                    {
                        throw std::runtime_error("Текст не может быть пустым.");
                    }
                    break;
                }
                if (inputMode == 2)
                {
                    std::cout << "Путь и имя файла: ";
                    std::string filename;
                    std::getline(std::cin, filename);

                    std::cout << "Тип файла:\n1. Текстовый\n2. Бинарный\n0. Назад\n> ";
                    int fileType = 0;
                    std::cin >> fileType;
                    clearCin();

                    if (fileType == 0)
                    {
                        continue;
                    }

                    try
                    {
                        if (fileType == 1)
                        {
                            data.text = readFile(filename);
                        }
                        else if (fileType == 2)
                        {
                            std::string raw = readBinaryFile(filename);

                            data.text = bytesToHex(raw);
                        }
                        else
                        {
                            std::cout << "Неверный тип. Повторите.\n";
                            continue;
                        }
                        if (data.text.empty())
                        {
                            throw std::runtime_error("Файл пуст.");
                        }
                        break;
                    }
                    catch (const std::exception& e)
                    {
                        std::cerr << "[Ошибка файла]: " << e.what() << "\n";
                    }
                }
                else
                {
                    std::cout << "Неверный выбор! Повторите!\n";
                }
            }

            if (data.encrypt)
            {
                std::cout << "Введите ключ (Enter — автогенерация, 0 — назад): ";
            }
            else
            {
                std::cout << "Введите ключ (0 — назад): ";
            }
            std::getline(std::cin, data.key);

            if (data.key == "0")
            {
                return {};
            }

            // генерация только при шифре
            if (data.key.empty())
            {
                if (data.encrypt)
                {
                    data.key = autoGenerateKey(cipher.name, data.text.length());
                    std::cout << "[Автоключ]: " << data.key << std::endl;
                }
                else
                {
                    throw std::runtime_error("Ключ обязателен при дешифровке.");
                }
            }

            return data;
        }
        catch (const std::exception& e)
        {
            std::cerr << "[Ошибка ввода]: " << e.what() << "\nПопробуйте снова.\n";
            userPause();
        }
    }
}

bool outputResult(const std::string& result, bool encrypt)
{
    while (true)
    {
        try
        {
            std::cout << "Вывод:\n1. На экран\n2. В файл\n0. Назад\n> ";
            int outputChoice = 0;
            std::cin >> outputChoice;
            clearCin();

            if (outputChoice == 0)
            {
                return false;
            }

            if (outputChoice == 1)
            {
                std::cout << (encrypt ? "Результат: " : "Расшифрованный текст: ") << result << "\n";
                userPause();
                return true;
            }
            if (outputChoice == 2)
            {
                std::cout << "Путь и имя выходного файла: ";
                std::string filename;
                std::getline(std::cin, filename);

                std::cout << "Тип файла:\n1. Текстовый \n2. Бинарный\n0. Назад\n> ";
                int fileType = 0;
                std::cin >> fileType;
                clearCin();

                if (fileType == 0)
                {
                    continue;
                }

                try
                {
                    if (fileType == 1)
                    {
                        writeFile(filename, result);
                    }
                    else if (fileType == 2)
                    {
                        std::string raw = hexToBytes(result);
                        writeBinaryFile(filename, raw);
                    }
                    else
                    {
                        std::cout << "Неверный тип. Повторите.\n";
                        continue;
                    }

                    std::cout << "Результат успешно сохранён.\n";
                    userPause();
                    return true;
                }
                catch (const std::exception& e)
                {
                    std::cerr << "[Ошибка вывода]: " << e.what() << "\n";
                    userPause();
                    continue;
                    ;
                }
            }
            else
            {
                std::cout << "Неверный выбор. Повторите.\n";
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "[Ошибка вывода]: " << e.what() << "\nПопробуйте снова.\n";
            userPause();
        }
    }
}
