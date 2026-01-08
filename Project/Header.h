#include <vector>
#include <iostream>
#include <string>

struct SudokuGrid {
	std::vector<std::vector<int>> grid;
	SudokuGrid() : grid(9, std::vector<int>(9, 0)) {}//конструктор, который заполняет поле 0
};

struct SudokuState {//структура состояний 
	int row, col;           // координаты ячейки
	SudokuGrid grid;        // копия сетки на этом шаге
	int num;               // текущее число для проверки (от 1 до 9)
};

struct CellPosition {//координаты пустой клетки
	int row;
	int col;
};

struct Sudoku {
	CellPosition getNextEmptyCell(const SudokuGrid& Sgrid, int startRow = 0, int startCol = 0);
	bool CanPlace(SudokuGrid& Sgrid, int row, int col, int num);
	bool SolveSudoku(SudokuGrid& Sgrid);
	void PrintGrid(SudokuGrid& Sgrid);
	void RemoveRandomNumber(SudokuGrid& Sgrid);
	int RandomNumber();
	bool CheckGrid(SudokuGrid& Sgrid);
	void SaveGrid(SudokuGrid& Sgrid);
	void LoadGrid(SudokuGrid& Sgrid);
	bool GenerateSudoku(SudokuGrid& Sgrid);
};
void PrintSolvedSudoku(SudokuGrid& grid);
void menu(SudokuGrid& grid);