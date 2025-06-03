#include "module_load.h"
#include <filesystem>
#include <dlfcn.h>
#include <iostream>
using namespace std;

vector<CipherPlugin> loadPlugins(const string& directory) {
    vector<CipherPlugin> plugins;

    for (const auto& entry : filesystem::directory_iterator(directory)) {
        if (entry.path().extension() != ".so") continue;

        void* handle = dlopen(entry.path().c_str(), RTLD_LAZY);
        if (!handle) {
            cerr << "Ошибка загрузки: " << dlerror() << "\n";
            continue;
        }

        auto get_name = (const char* (*)()) dlsym(handle, "get_name");
        auto get_description = (const char* (*)()) dlsym(handle, "get_description");
        auto encrypt = (const char* (*)(const char*, const char*)) dlsym(handle, "encrypt");
        auto decrypt = (const char* (*)(const char*, const char*)) dlsym(handle, "decrypt");

        if (!get_name || !encrypt || !decrypt) {
            cerr << "Битый плагин: " << entry.path() << "\n";
            dlclose(handle);
            continue;
        }

        string name = get_name();
        string desc = get_description ? get_description() : "Описания нет";

        plugins.push_back(CipherPlugin{
            handle,
            name,
            desc,
            encrypt,
            decrypt
        });
    }

    return plugins;
}


void unload_plugins(vector<CipherPlugin>& plugins) {
    for (auto& plugin : plugins) {
        if (plugin.handle) {
            dlclose(plugin.handle);
            plugin.handle = nullptr;
        }
    }
    plugins.clear();
}