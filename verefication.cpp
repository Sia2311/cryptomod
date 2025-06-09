#include <array>
#include <cstring>
#include <string>
#include <vector>

#include "utils.h"

// clang-format off
const std::array<unsigned char, 64> expectedBytes= {
    0xff, 0xd8, 0xff, 0xe0, 0x00, 0x10, 0x4a, 0x46,
    0x49, 0x46, 0x00, 0x01, 0x01, 0x01, 0x00, 0x60,
    0x00, 0x60, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x3b,
    0x43, 0x52, 0x45, 0x41, 0x54, 0x4f, 0x52, 0x3a,
    0x20, 0x67, 0x64, 0x2d, 0x6a, 0x70, 0x65, 0x67,
    0x20, 0x76, 0x31, 0x2e, 0x30, 0x20, 0x28, 0x75,
    0x73, 0x69, 0x6e, 0x67, 0x20, 0x49, 0x4a, 0x47,
    0x20, 0x4a, 0x50, 0x45, 0x47, 0x20, 0x76, 0x38
};
// clang-forma on
bool verifyImageKey(const std::string& path)
{
    std::string data;
    try{
        data = readBinaryFile(path);
    }catch (...){
        return false;
    }
    if(data.size() < sizeof(expectedBytes)){
        return false;
    }

    for (size_t i = 0; i < sizeof(expectedBytes); ++i) {
        if (static_cast<unsigned char>(data[i]) != expectedBytes.at(i)) {
            return false;
        }
    }

    return true;
}

const std::vector<std::string> allowedUUIDS = { "B4FE-5315" };

// Проверка, подключена ли флешка с одним из известных UUID
bool checkUSB() {
    FILE* filePointer = popen("lsblk -o UUID", "r");
    if (filePointer == nullptr) { return false;
}

    std::array<char, 256> buffer{};
    while (fgets(buffer.data(), buffer.size(), filePointer) != nullptr) {
        for (const auto& uuid : allowedUUIDS) {
            if (strstr(buffer.data(), uuid.c_str()) != nullptr) {
                pclose(filePointer);
                return true;
            }
        }
    }

    pclose(filePointer);
    return false;
}