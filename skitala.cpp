#include "CipherInterface.h"
#include <string>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;
//заполняем по строка читаем по столбцам
string encrypt_skitale(const string& text, int columns){

    if(text.empty() || columns <= 0) return "";
    //вычисляем кол-во строк длина текста / столбцы
    int rows = static_cast<int>(ceil((double)text.length() / columns));

    vector<vector<char>> grid(rows, vector<char>(columns, ' '));
    // заполняем таблицу
    int index = 0;
    for(int r = 0; r < rows && index < text.length(); ++r){
        for(int c = 0; c < columns && index < text.length(); ++c){
            grid[r][c] = text[index++];
        }
    }
    //читаем по столбцам , тоесть сверху вниз и слева направо
    string result;
    for(int c = 0; c < columns; ++c){
        for (int r = 0; r < rows; ++r){
            result += grid[r][c];
        }
    }
    return result;
}
// заполняем по столбцам, читаем по строкам
string decrypt_skitale(const string& text, int columns){

     //вычисляем кол-во строк длина текста / столбцы
     int rows = static_cast<int>(ceil((double)text.length() / columns));
     int totalCells = rows * columns;
     int pad = totalCells - text.length(); // скольки лишних ячеек

     vector<vector<char>> grid(rows, vector<char>(columns, ' '));

     int index = 0;

     //теперь по столбцаm
     for(int c =0; c < columns && index < text.length(); ++c){
        for (int r = 0; r < rows && index < text.length(); ++r){
            //последние ячейки в нижней строке остаются пустыми
            if (c >= columns - pad && r == rows - 1) continue;
            grid[r][c] = text[index++];
        }
     }
     // читаем по строкам
     string result;
     for(int r = 0; r < rows; ++r){
        for (int c = 0; c < columns; ++c){
            result +=grid[r][c];
        }
     }
     return result;
}
extern "C"{
    const char* encrypt(const char* text, const char* key){
        if(!text || !key) return nullptr;

        int columns;
        try{
            columns = stoi(key);
        } catch(...){
            fprintf(stderr, "[Скитала] Ключ должен быть число!\n");
            return nullptr;
        }
        string result = encrypt_skitale(text, columns);
        return strdup(result.c_str()); // кидаем в кучу
    }
    const char* decrypt(const char* text, const char* key){

        int columns;
        try{
            columns = stoi(key);
        }catch (...){
            fprintf(stderr, "[Скитал] Ключ должен быть целым числом!");
            return nullptr;
        }
        string result = decrypt_skitale(text,columns);
        return strdup(result.c_str());
    }
    const char* get_name() {
        return "Скитала (перестановка)";
    }
    const char* get_description(){
        return "Шифр перестановки";
    }
}