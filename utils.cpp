#include <iostream>
#include <limits>
#include "utils.h"
using namespace std;
void pause() {
    cout << "\nНажмите Enter для возврата...";
    cin.ignore();
    cin.get();
}

void clearCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}