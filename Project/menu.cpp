#include <iostream>
#include <string>
#include <vector>
#include "Header.h"
void PrintSolvedSudoku(SudokuGrid& grid) {
	Sudoku solver;
	std::string input2;
	bool flag2 = true;
	int choice = 0;
	std::cout << "Куда вывести решённое судоку?" << std::endl;
	std::cout << "1 - в консоль" << std::endl;
	std::cout << "2 - в файл" << std::endl;
	std::cout << "3 - в консоль и файл" << std::endl;
    std::cout << "4 - вернуться в меню" << std::endl;
	while (flag2) {
		try {
			getline(std::cin, input2);
			choice = stoi(input2);
			flag2 = false;
		}
		catch (std::invalid_argument) {
			std::cerr << "Ошибка ввода. Попробуйте снова!" << std::endl;
            continue;
		}
		if (choice > 0 and choice < 5) {
			flag2 = false;
		}
		else {
			std::cerr << "Ошибка ввода. Попробуйте снова!" << std::endl;
		}
	}
	if (choice == 1) {
		std::cout << "Решённое судоку:" << std::endl;
		solver.PrintGrid(grid);
        menu(grid);
	}
	else if (choice == 2) {
		solver.SaveGrid(grid);
        menu(grid);
	}
	else if (choice == 3) {
		solver.SaveGrid(grid);
		std::cout << "Решённое судоку:" << std::endl;
		solver.PrintGrid(grid);
        menu(grid);
    }
    else if (choice == 4) {
        menu(grid);
    }

}
void menu(SudokuGrid& grid) {
    Sudoku solver; // создаём экземпляр решателя судоку
    std::cout << "<------------Menu----------->" << std::endl;
    std::cout << "1 - Проверить корректность поля  " << std::endl;
    std::cout << "2 - Решить судоку (итеративно) " << std::endl;
    std::cout << "3 - Сгенерировать новое судоку" << std::endl;
    std::cout << "4 - Создать задачу (удалить числа)" << std::endl;
    std::cout << "5 - Сохранить поле в файл" << std::endl;
    std::cout << "6 - Загрузить поле из файла" << std::endl;
    std::cout << "7 - выход" << std::endl;
    std::cout << "(пустое значение клетки в судоку отмечено точкой)" << std::endl;
    bool flag = true;
    int choice = 0;
    std::cout << "Введите выбранную опцию: ";
    std::string input;
    while (flag) {
        getline(std::cin, input);  
        // Проверка на наличие точки
        if (input.find('.') != std::string::npos) {
            std::cerr << "Дробные числа не допускаются!" << std::endl;
            continue;
        }
        try {
            choice = stoi(input);
        }
        catch (std::invalid_argument) {
            std::cerr << "Ошибка ввода, попробуйте снова!" << std::endl;
            continue;
        }
        if (choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5 || choice == 6 || choice == 7){
            flag = false;
        }
        else {
            std::cerr << "Ошибка ввода, попробуйте снова!" << std::endl;
        }
    }
    bool success = false;
    switch (choice) {

    case 1:
        if (solver.IsGridEmpty(grid)) {
            std::cout << "Поле пустое" << std::endl;
        }
        else if (solver.CheckGrid(grid)) {
            std::cout << "Поле корректно" << std::endl;
        }
        else {
            std::cout << "Поле не корректно" << std::endl;
        }
        std::cout << std::endl;
        menu(grid);  
        break;
    case 2: {
        if (!solver.IsGridEmpty(grid)) {
            solver.SolveSudoku(grid);
            solver.PrintGrid(grid);
        }
        else {
            std::cout << "Поле пустое" << std::endl;
        }
        std::cout << std::endl;
        menu(grid);
        break;
    }
    case 3: 
        do {
            success = solver.GenerateSudoku(grid);
        } while (!success);
        std::cout << "Сгенерированное судоку:" << std::endl;
        solver.PrintGrid(grid);
        std::cout << std::endl;
        menu(grid);
        break;
    case 4:
        if (!solver.IsGridEmpty(grid)) {
            solver.RemoveRandomNumber(grid);
            std::cout << "Созданная задача:" << std::endl;
            solver.PrintGrid(grid);
        }
        else {
            std::cout << "Поле пустое" << std::endl;
        }
        std::cout << std::endl;
        menu(grid);
        break;
    case 5:
        if (!solver.IsGridEmpty(grid)) {
            solver.SaveGrid(grid);
        }
        else {
            std::cout << "Поле пустое" << std::endl;
        }
        std::cout << std::endl;
        menu(grid);
        break;
    case 6:
        solver.LoadGrid(grid);
        if (!solver.IsGridEmpty(grid)) {
            std::cout << "Загруженное поле:" << std::endl;
            solver.PrintGrid(grid);
        }
        else {
            std::cout << "Поле пустое" << std::endl;
        }
        std::cout << std::endl;
        menu(grid);
        break;
    case 7:
        break;
    default:
        std::cout << "Ошибка ввода" << std::endl;
        break;
    }
}