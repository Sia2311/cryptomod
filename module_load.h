#pragma once

#include <string>
#include <vector>
using namespace std;

struct CipherPlugin{
    void* handle;
    string name;
    string description;

    const char* (*encrypt)(const char*, const char*);
    const char* (*decrypt)(const char*, const char*);
};

vector<CipherPlugin> loadPlugins(const string& directory);
void unload_plugins(vector<CipherPlugin>& plugins);