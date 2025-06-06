#include "module_load.h"
#include <filesystem>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
    #define LibHandle HMODULE
    #define libLoad(path) LoadLibraryA(path)
    #define libFunc GetProcAddress
    #define libFree FreeLibrary
    #define LIB_EXT ".dll"
#else
    #include <dlfcn.h>
    #define LibHandle void*
    #define libLoad(path) dlopen(path, RTLD_LAZY)
    #define libFunc dlsym
    #define libFree dlclose
    #define LIB_EXT ".so"
#endif

using namespace std;

vector<CipherPlugin> loadPlugins(const string& directory) {
    vector<CipherPlugin> plugins;

    for (const auto& entry : filesystem::directory_iterator(directory)) {
        if (entry.path().extension() != LIB_EXT)
            continue;

        string pathStr = entry.path().string();
        LibHandle handle = libLoad(pathStr.c_str());

        if (!handle) {
#ifdef _WIN32
            cerr << "Ошибка загрузки DLL: " << pathStr << "\n";
#else
            cerr << "Ошибка загрузки SO: " << dlerror() << "\n";
#endif
            continue;
        }

        auto get_name = (const char* (*)()) libFunc(handle, "get_name");
        auto get_description = (const char* (*)()) libFunc(handle, "get_description");
        auto encrypt = (const char* (*)(const char*, const char*)) libFunc(handle, "encrypt");
        auto decrypt = (const char* (*)(const char*, const char*)) libFunc(handle, "decrypt");

        if (!get_name || !encrypt || !decrypt) {
            cerr << "Битый плагин: " << entry.path() << "\n";
            libFree(handle);
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
            libFree(plugin.handle);
            plugin.handle = nullptr;
        }
    }
    plugins.clear();
}
