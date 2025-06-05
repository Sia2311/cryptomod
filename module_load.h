#pragma once
#include <string>
#include <vector>
using namespace std;

#ifdef _WIN32
    #include <windows.h>
    using LibHandle = HMODULE;
#else
    using LibHandle = void*;
#endif

struct CipherPlugin {
    LibHandle handle;
    string name;
    string description;
    const char* (*encrypt)(const char*, const char*) = nullptr;
    const char* (*decrypt)(const char*, const char*) = nullptr;
};

vector<CipherPlugin> loadPlugins(const string& directory);
void unload_plugins(vector<CipherPlugin>& plugins);
