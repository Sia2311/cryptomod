#include "module_load.h"

#include <dlfcn.h>

#include <filesystem>
#include <iostream>

std::vector<CipherPlugin> loadPlugins(const std::string& directory)
{
    std::vector<CipherPlugin> plugins;

    // загружаем сошники
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.path().extension() != ".so")
        {
            continue;
        }
        // путь в строку
        std::string pathStr = entry.path().string();
        void* handle = dlopen(pathStr.c_str(), RTLD_LAZY);

        if (handle == nullptr)
        {
            std::cerr << "Ошибка загрузки SO: " << dlerror() << "\n";
            continue;
        }

        auto getName = reinterpret_cast<const char* (*) ()>(dlsym(handle, "getName"));
        auto getDescription = reinterpret_cast<const char* (*) ()>(dlsym(handle, "getDescription"));
        auto encrypt = reinterpret_cast<const char* (*) (const char*, const char*)>(dlsym(handle, "encrypt"));
        auto decrypt = reinterpret_cast<const char* (*) (const char*, const char*)>(dlsym(handle, "decrypt"));

        if ((getName == nullptr) || (encrypt == nullptr) || (decrypt == nullptr))
        {
            std::cerr << "Битый плагин: " << entry.path() << "\n";
            dlclose(handle);
            continue;
        }

        std::string name = getName();
        std::string desc = (getDescription != nullptr) ? getDescription() : "Описания нет";

        plugins.push_back(CipherPlugin{handle, name, desc, encrypt, decrypt});
    }

    return plugins;
}

void unloadPlugins(std::vector<CipherPlugin>& plugins)
{
    for (auto& plugin : plugins)
    {
        if (plugin.handle != nullptr)
        {
            dlclose(plugin.handle);
            plugin.handle = nullptr;
        }
    }
    plugins.clear();
}
