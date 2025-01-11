#include "Simulation.h"

Simulation::Simulation(int rows, int columns) : s(rows, columns) {}

Matrix& Simulation::get_matrix() { return s; }

void Simulation::streaming() {
    Matrix next_matrix = s;
    int rows = s.get_rows_num();
    int columns = s.get_columns_num();

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < columns; j++) {
            if (s.get_element(i, j).get_fun(FUN_IN) != WALL) {
                next_matrix.get_element(i, j).set_fun(FUN_IN, EMPTY);
                next_matrix.get_element(i, j).set_fun(FUN_EQ, EMPTY);
                next_matrix.get_element(i, j).set_fun(FUN_EQ, EMPTY);
            }
        }
    }


    for (size_t i = 1; i < rows - 1; i++) {
        for (size_t j = 1; j < columns - 1; j++) {
            Cell& current_cell = s.get_element(i, j);
            array<double, 9> fun_ex = current_cell.get_fun(FUN_EX);

            for (int d = 0; d < 9; d++) {
                if (fun_ex[d] != 0) {
                    int ni = i + VELOCITY_FACTOR[d][0];
                    int nj = j + VELOCITY_FACTOR[d][1];
                    int opposite_d = OPPOSITE_DIRECTION[d];

                    if (ni >= 0 && ni < rows && nj >= 0 && nj < columns) {
                        Cell& target_cell = next_matrix.get_element(ni, nj);
                        if (target_cell.get_fun(FUN_IN) != WALL) {
                            target_cell.set_direct_fun(FUN_IN, d, target_cell.get_fun(FUN_IN)[d] + fun_ex[d]);
                        }
                        else {
                            current_cell.set_direct_fun(FUN_IN, opposite_d, current_cell.get_fun(FUN_IN)[opposite_d] + fun_ex[d]);
                        }
                    }
                }
            }
        }
    }

    s = next_matrix;
}


void Simulation::collision() {
    Matrix next_matrix = s;
    int rows = s.get_rows_num();
    int columns = s.get_columns_num();

    for (size_t i = 1; i < rows - 1; i++) {
        for (size_t j = 1; j < columns - 1; j++) {
            Cell& next_cell = next_matrix.get_element(i, j);
            if (s.get_element(i, j).get_fun(FUN_IN) != WALL) {
                next_cell.calculate_density();
                next_cell.calculate_velocity();
                next_cell.calculate_fun_eq();
                next_cell.calculate_fun_ex();
            }
        }
    }

    s = next_matrix;
}