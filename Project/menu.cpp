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
    std::cout << "1 - загрузить судоку из файла" << std::endl;
    std::cout << "2 - сгенерировать судоку" << std::endl;
    std::cout << "3 - выход" << std::endl;
    std::cout << "(пустое значение клетки в судоку отмечено точкой)" << std::endl;
    bool flag = true;
    int choice = 0;
    std::cout << "Введите выбранную опцию: ";
    std::string input;
    while (flag) {
        try {
            getline(std::cin, input);
            choice = stoi(input);
        }
        catch (std::invalid_argument) {
            std::cerr << "Ошибка ввода, попробуйте снова!" << std::endl;
            continue;
        }
        if (choice == 1 || choice == 2 || choice == 3 || choice == 4) {
            flag = false;
        }
        else {
            std::cerr << "Ошибка ввода, попробуйте снова!" << std::endl;
        }
    }
    switch (choice) {

    case 1:
        solver.LoadGrid(grid);
        std::cout << "Загруженное судоку" << std::endl;
        solver.PrintGrid(grid);
        if (solver.CheckGrid(grid)) {
            solver.SolveSudoku(grid);
            PrintSolvedSudoku(grid);
            break;
        }
        else {
            std::cout << "Поле не корректно" << std::endl;
            menu(grid);
            break;
        }
    case 2: {
        bool success = false;
        do {
            // Пытаемся сгенерировать судоку
            success = solver.GenerateSudoku(grid);
        } while (!success);
        std::cout << "Сгенерированное судоку" << std::endl;
        solver.RemoveRandomNumber(grid);
        solver.PrintGrid(grid);
        if (solver.CheckGrid(grid)) {
            solver.SaveGrid(grid);
            menu(grid);
            break;
        }
        else {
            std::cout << "Поле не корректно" << std::endl;
            menu(grid);
        }
        break;
    }
    case 3: break;
    default:
        std::cout << "Ошибка ввода" << std::endl;
        break;
    }
}