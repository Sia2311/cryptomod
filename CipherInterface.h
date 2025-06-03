#pragma once

extern "C"{

    const char* encrypt(const char* inputText, const char* key);

    const char* decrypt(const char* cipherText, const char* key);
    
    const char* get_name();

    const char* get_description();

}