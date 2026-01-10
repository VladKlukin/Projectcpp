#include <iostream>
#include <string>
#include <vector>
#include "Header.h"
#include <ctime>      
#include <cstdlib>    
#include <fstream>
#include <sstream>
// Проверка возможности размещения числа в ячейке
bool Sudoku::CanPlace(SudokuGrid& Sgrid, int row, int col, int num) {
	// Проверка границ индексов
	if (row < 0 || row > 8 || col < 0 || col > 8) {
		std::cerr << "Индексы строки и столбца должны быть в диапазоне 0-8" << std::endl;
		return false;
	}

	// Проверка допустимого значения числа
	if (num < 1 || num > 9) {
		std::cerr << "Число должно быть в диапазоне 1-9" << std::endl;
		return false;
	}

	for (int x = 0; x < 9; ++x) {
		if (Sgrid.grid[row][x] == num) return false;
	}

	// Проверяем столбец
	for (int y = 0; y < 9; ++y) {
		if (Sgrid.grid[y][col] == num) return false;
	}

	// Проверяем блок
	int startRow = row - row % 3;
	int startCol = col - col % 3;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
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
				std::cerr << "Некорректное значение в ячейке" << std::endl;
				return { -1, -1 };
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
	std::cout << "Сколько чисел вы хотите удалить?(От 10 до 50) " << std::endl;
	bool flag = true;
	std::string input;

	while (flag) {
		getline(std::cin, input);

		// Проверка на пустой ввод
		if (input.empty()) {
			std::cerr << "Введите число!" << std::endl;
			continue;
		}
		
		// Проверка на наличие букв в строке
		bool hasLetters = false;
		for (char c : input) {
			if (std::isalpha(c)) {
				hasLetters = true;
				break;
			}
		}
		if (hasLetters) {
			std::cerr << "Ввод содержит буквы! Допустимы только цифры." << std::endl;
			continue;
		}

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
		catch (std::out_of_range) {
			std::cerr << "Число выходит за допустимый диапазон!" << std::endl << "Число должно быть в диапазоне от 10 до 50!" << std::endl;
			std::cout << "Попробуйте снова!" << std::endl;
			continue;
		}
		if (count <= 50 && count >= 10) {
			flag = false;
		}
		else {
			std::cerr << "Ошибка! Число должно быть в диапазоне от 10 до 50! " << std::endl;
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
		return;
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
		std::cerr << "Ошибка открытия файла " << filename << std::endl;
		Flag = false;
		return;
	}

	// 1. Считываем все строки в вектор строк для предварительной проверки
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(file, line)) {
		lines.push_back(line);
	}
	file.close(); // Закрываем файл для повторного чтения

	// 2. Проверка количества строк (должно быть 9)
	if (lines.size() != 9) {
		std::cerr << "Ошибка: должно быть 9 строк, найдено " << lines.size() << std::endl;
		Flag = false;
		return;
	}

	// 3. Проверка количества чисел в каждой строке и диапазона значений
	std::vector<std::vector<int>> tempGrid(9, std::vector<int>(9)); // Временная сетка для проверки
	for (int i = 0; i < 9; i++) {
		std::stringstream ss(lines[i]);
		int num, count = 0;
		bool hasExtraNumbers = false;

		while (ss >> num) {
			if (count >= 9) {
				std::cerr << "Ошибка: в строке " << (i + 1)
					<< " найдено больше 9 чисел" << std::endl;
				Flag = false;
				return;
			}

			if (num < 0 || num > 9) {
				std::cerr << "Ошибка: число " << num << " в строке " << (i + 1)
					<< " вне допустимого диапазона [0–9]" << std::endl;
				Flag = false;
				return;
			}
			tempGrid[i][count] = num;
			count++;
		}

		if (count < 9) {
			std::cerr << "Ошибка: в строке " << (i + 1) << " только "
				<< count << " чисел (должно быть 9)" << std::endl;
			Flag = false;
			return;
		}
	}

	// 4. Если проверка пройдена — заполняем реальную сетку Sgrid
	file.open(filename); // Открываем файл заново для заполнения сетки
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			file >> Sgrid.grid[i][j];
		}
	}
	file.close();

	std::cout << "Сетка успешно загружена!" << std::endl;
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
	// Создаем массив возможных чисел 
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

