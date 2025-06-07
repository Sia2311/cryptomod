#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "utils.h"

using namespace std;

const unsigned char expectedBytes[] = {
    0xff, 0xd8, 0xff, 0xe0, 0x00, 0x10, 0x4a, 0x46,
    0x49, 0x46, 0x00, 0x01, 0x01, 0x01, 0x00, 0x60,
    0x00, 0x60, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x3b,
    0x43, 0x52, 0x45, 0x41, 0x54, 0x4f, 0x52, 0x3a,
    0x20, 0x67, 0x64, 0x2d, 0x6a, 0x70, 0x65, 0x67,
    0x20, 0x76, 0x31, 0x2e, 0x30, 0x20, 0x28, 0x75,
    0x73, 0x69, 0x6e, 0x67, 0x20, 0x49, 0x4a, 0x47,
    0x20, 0x4a, 0x50, 0x45, 0x47, 0x20, 0x76, 0x38
};

bool verifyImageKey(const string& path) {
    string data;
    try{
        data = readBinaryFile(path);
    }catch (...){
        return false;
    }
    if(data.size() < sizeof(expectedBytes)){
        return false;
    }

    for (size_t i = 0; i < sizeof(expectedBytes); ++i) {
        if (static_cast<unsigned char>(data[i]) != expectedBytes[i]) {
            return false;
        }
    }

    return true;
}

const vector<string> allowedUUIDs = {
    "B4FE-5315" 
};

// Проверка, подключена ли флешка с одним из известных UUID
bool checkUSB() {
    FILE* fp = popen("lsblk -o UUID", "r");
    if (!fp) return false;

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp)) {
        for (const auto& uuid : allowedUUIDs) {
            if (strstr(buffer, uuid.c_str())) {
                pclose(fp);
                return true;
            }
        }
    }

    pclose(fp);
    return false;
}
