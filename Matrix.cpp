#include "Matrix.h"

Matrix::Matrix(int rows, int columns) : rows(rows), columns(columns), element(rows, vector<Cell>(columns)) {
	if (rows <= 0 || columns <= 0) {
		throw invalid_argument("Matrix dimensions must be greater than zero.");
	}
}

Matrix::Matrix(const Matrix& other) : rows(other.rows), columns(other.columns), element(other.element) {}

int Matrix::get_rows_num() const { return rows; }
int Matrix::get_columns_num() const { return columns; }

Cell& Matrix::get_element(int i, int j) {
	if (i < 0 || i >= rows || j < 0 || j >= columns) throw out_of_range("Index out of range in get_element.");
	return element[i][j];
}

void Matrix::prepare_environment() {
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < columns; j++) {
			Cell& currentCell = element[i][j];
			if (i == 0 || i == rows - 1 || j == 0 || j == columns - 1 || (j == columns / 3 && (i < rows / 2 - 3 || i > rows / 2 + 3))) currentCell.set_fun(FUN_IN,WALL);
			else {
				if (i > 0 && i < rows - 1 && j > 0 && j < columns / 3) currentCell.set_density(1.0);
				else currentCell.set_density(0.96);

				currentCell.calculate_fun_eq(); 
				currentCell.set_fun(FUN_IN, currentCell.get_fun(FUN_EQ));
			}
		}
	}
}