#include <iostream>
#include <limits>
#include "utils.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

string readFile(const string& filename){
    ifstream file(filename);
    if(!file){
        cerr << "Не удалось открыть файл" <<  filename << endl;
        return "";
    }
    stringstream copy;
    copy << file.rdbuf();
    return copy.str();
}

bool writeFile(const string& filename, const string& text){
    ofstream file(filename);
    if(!file){
        cerr << "Не удалось открыть файл" << filename << endl;
        return false;
    }
    file << text;
    return true;
}

void pause() {
    cout << "\nНажмите Enter для возврата...";
    cin.ignore();
    cin.get();
}

void clearCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}