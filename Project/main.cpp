#include <iostream>
#include <string>
#include "Header.h" 
#include <ctime>
#include <clocale>
int main() {
    setlocale(LC_ALL, "rus");
    srand(time(NULL)); // инициализация генератора случайных чисел
    SudokuGrid grid; // создаём пустую сетку 9x9
    std::cout << "<---Интерактивный решатель судоку--->" << std::endl;
    menu(grid);
    return 0;
}
