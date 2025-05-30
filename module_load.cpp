#include "module_load.h"
#include <filesystem>
#include <dlfcn.h>
#include <iostream>
using namespace std;
// создаём вектор всех загруженных плагинов
vector<CipherPLugin> loadPlugins(const string& directory){
    vector<CipherPLugin> plugins;

    //бежим по папке и оставляем только .so
    for(const auto& entry : filesystem::directory_iterator(directory)){

        if(entry.path().extension() != ".so") continue;
        // загружаем .so в память
        void* handle = dlopen(entry.path().c_str(), RTLD_LAZY);

        if(!handle){
            cerr << "Ошибка: " << dlerror() << "\n";
            continue;
        }
        // вытаскиваем указатели функций
        auto get_name = (const char* (*)()) dlsym(handle, "get_name");
        auto get_decription = (const char* (*)()) dlsym(handle, "get_description");
        auto encrypt = (const char* (*)(const char*, const char*)) dlsym(handle, "encrypt");
        auto decrypt = (const char* (*)(const char*, const char*)) dlsym(handle, "decrypt");

        //если какой-то функции нет
        if(!get_name || !encrypt){
            cerr << "Битый плагин: " << entry.path() << "\n";
            dlclose(handle);
        }
        //сохраняем работающий плагин
        plugins.push_back(CipherPLugin{
            handle, get_name(),
            get_decription ? get_decription() : "Описания нет",
            encrypt
        });
    }
    return plugins;
}

// освобождаем память связанную с .so
void unload_plugins(vector<CipherPLugin>& plugins){
    for (auto& plugin : plugins){
        if(plugin.handle){
            dlclose(plugin.handle);
            plugin.handle = nullptr;
        }
    }
    plugins.clear();
}