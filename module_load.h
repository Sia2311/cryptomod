#pragma once
#include <string>
#include <vector>

struct CipherPlugin
{
    void* handle;
    std::string name;
    std::string description;
    const char* (*encrypt)(const char*, const char*) = nullptr;
    const char* (*decrypt)(const char*, const char*) = nullptr;
};

std::vector<CipherPlugin> loadPlugins(const std::string& directory);
void unloadPlugins(std::vector<CipherPlugin>& plugins);
