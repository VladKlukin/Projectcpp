#include <iostream>
#include <string>
#include <vector>
#include "Header.h"
#include <ctime>      
#include <cstdlib>    
#include <fstream>
// Проверка возможности размещения числа в ячейке
bool Sudoku::CanPlace(SudokuGrid& Sgrid, int row, int col, int num) {
	// Проверка границ индексов
	if (row < 0 || row > 8 || col < 0 || col > 8) {
		throw std::out_of_range("Индексы строки и столбца должны быть в диапазоне 0-8");
	}

	// Проверка допустимого значения числа
	if (num < 1 || num > 9) {
		throw std::invalid_argument("Число должно быть в диапазоне 1-9");
	}

	for (int x = 0; x < 9; x++) {
		if (Sgrid.grid[row][x] == num) return false;
	}

	// Проверяем столбец
	for (int y = 0; y < 9; y++) {
		if (Sgrid.grid[y][col] == num) return false;
	}

	// Проверяем блок
	int startRow = row - row % 3;
	int startCol = col - col % 3;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (Sgrid.grid[startRow + i][startCol + j] == num) return false;
		}
	}
	return true;
}

// Основной решатель судоку
bool Sudoku::SolveSudoku(SudokuGrid& Sgrid) {
	std::vector<SudokuState> states;
	auto [startRow, startCol] = getNextEmptyCell(Sgrid);
	if (startRow == -1) return true; // сетка уже заполнена
	states.push_back({ startRow, startCol, Sgrid, 1 });
	while (!states.empty()) {
		auto& curr = states.back();

		// Если число не подходит или исчерпали варианты
		if (curr.num > 9) {
			states.pop_back();
			if (states.empty()) return false; // нет решений

			// Откатываемся: очищаем ячейку и пробуем следующее число
			Sgrid.grid[curr.row][curr.col] = 0;
			++states.back().num;
			continue;
		}

		// Проверяем, можно ли поставить число
		if (CanPlace(Sgrid, curr.row, curr.col, curr.num)) {
			Sgrid.grid[curr.row][curr.col] = curr.num;

			auto [nextRow, nextCol] = getNextEmptyCell(Sgrid, curr.row, curr.col);
			if (nextRow == -1) {
				return true; // сетка заполнена — решение найдено
			}

			// Добавляем следующее состояние
			states.push_back({ nextRow, nextCol, Sgrid, 1 });
		}
		else {
			// Число не подходит — пробуем следующее
			++curr.num;
		}
	}
	return false;
}

CellPosition Sudoku::getNextEmptyCell(const SudokuGrid& Sgrid, int startRow, int startCol) { 
    for (int row = startRow; row < 9; ++row) {
        for (int col = (row == startRow ? startCol : 0); col < 9; ++col) {
            if (Sgrid.grid[row][col] < 0 || Sgrid.grid[row][col] > 9) {
                throw std::invalid_argument("Некорректное значение в ячейке");
            }
            if (Sgrid.grid[row][col] == 0) {
                return { row, col };
            }
        }
    }
    return { -1, -1 };
}

// Вывод сетки судоку в консоль
void Sudoku::PrintGrid(SudokuGrid& Sgrid) {
	for (int row = 0; row < 9; ++row) {
		std::cout << "|";
		for (int col = 0; col < 9; ++col) {
			if (Sgrid.grid[row][col] == 0) {
				std::cout << " . ";
			}
			else {
				std::cout << " " << Sgrid.grid[row][col] << " ";
			}
			

		}
		std::cout << std::endl;
		if (row % 3 == 2 ) { // Разделители между блоками 3x3
			std::cout << "---------+--------+--------" << std::endl;
		}
		
	}
	
}

int Sudoku::RandomNumber() {
	return rand() % 9;
}

// Создание пустых ячеек для получения решаемой головоломки
void Sudoku::RemoveRandomNumber(SudokuGrid& Sgrid) {
	int count = 0;
	std::cout << "Сколько чисел вы хотите удалить?(Не больше 50) " << std::endl;
	bool flag = true;
	std::string input;
	while (flag) {
		getline(std::cin, input);
		// Проверка на наличие точки
		if (input.find('.') != std::string::npos) {
			std::cerr << "Дробные числа не допускаются!" << std::endl;
			continue;
		}
		try {
			count = stoi(input);
		}
		catch (std::invalid_argument) {
			std::cerr << "Ошибка ввода. Попробуйте снова!" << std::endl;
			continue;
		}
		if (count <= 50 && count >= 1) {
			flag = false;
		}
		else {
			std::cerr << "Ошибка! Число должно быть в диапазоне от 1 до 50! " << std::endl;
			continue;
		}
	}
	int cnt = 0;
	int row = 0;
	int col = 0;
	while (cnt != count) {
		row = RandomNumber();
		col = RandomNumber();
		if (Sgrid.grid[row][col] != 0) {  // Убеждаемся, что ячейка не пустая
			Sgrid.grid[row][col] = 0;
			++cnt;
		}
	}
}

// Проверка корректности заполненной сетки
bool Sudoku::CheckGrid(SudokuGrid& Sgrid) {
	for (int row = 0; row < 9; ++row) {
		for (int col = 0; col < 9; ++col) {
			if (Sgrid.grid[row][col] != 0) { // если ячейка заполнена
				int num = Sgrid.grid[row][col];
				Sgrid.grid[row][col] = 0; // временно очищаем для проверки
				if (!CanPlace(Sgrid, row, col, num)) {
					Sgrid.grid[row][col] = num; // возвращаем значение
					return false; // поле некорректно
				}
				Sgrid.grid[row][col] = num; // возвращаем значение
			}
		}
	}
	return true; // поле корректно
}

// Сохранение сетки в файл
void Sudoku::SaveGrid(SudokuGrid& Sgrid) {
	std::string filename = "Save.txt";
	std::ofstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Ошибка открытия файла " << filename << std::endl;
		menu(Sgrid);
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			file << Sgrid.grid[i][j] << " ";
		}
		file << std::endl;
	}
	std::cout << "Судоку сохранено в " << filename << std::endl;
	file.close();
}


// Загрузка сетки из файла	
void Sudoku::LoadGrid(SudokuGrid& Sgrid) {
	std::string filename = "Load.txt";
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Ошибка получения " << filename << std::endl;
		menu(Sgrid);
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			file >> Sgrid.grid[i][j];
		}
	}
	file.close();
}

//проверка на пустое поле
bool Sudoku::IsGridEmpty(const SudokuGrid& Sgrid) {
	for (int row = 0; row < 9; ++row) {
		for (int col = 0; col < 9; ++col) {
			if (Sgrid.grid[row][col] != 0) {
				return false; // Найдена заполненная ячейка — сетка не пуста
			}
		}
	}
	return true; // Все ячейки равны 0 — сетка пуста
}

// Генерация полной сетки судоку
bool Sudoku::GenerateSudoku(SudokuGrid& Sgrid) {
	// Инициализируем сетку нулями
	for (auto& row : Sgrid.grid) {
		std::fill(row.begin(), row.end(), 0);
	}
	// Инициализация генератора случайных чисел только один раз
	static bool initialized = false;
	if (!initialized) {
		srand(time(NULL));
		initialized = true;
	}

	// Создаем массив возможных чисел один раз
	std::vector<int> nums = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::vector<bool> used(9, false); // Для отслеживания использованных чисел

	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			// Перемешиваем числа вручную
			std::vector<int> shuffledNums = nums;
			for (int i = 0; i < 9; i++) {
				int randomIndex = rand() % (9 - i);
				std::swap(shuffledNums[i], shuffledNums[i + randomIndex]);
			}

			bool placed = false;
			for (int num : shuffledNums) {
				if (CanPlace(Sgrid, row, col, num)) {
					Sgrid.grid[row][col] = num;
					placed = true;
					break;
				}
			}

			if (!placed) {
				// Если число не удалось разместить - откатываемся
				return false;
			}
		}
	}
	return true;
}

