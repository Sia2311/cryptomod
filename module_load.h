#pragma once

#include <string>
#include <vector>
using namespace std;

struct CipherPLugin{
    void* handle;
    string name;
    string description;

    const char* (*encrypt)(const char*, const char*);
    const char* (*decrypt)(const char*, const char*);
};

vector<CipherPLugin> loadPlugins(const string& directory);
void unload_plugins(vector<CipherPLugin>& plugins);