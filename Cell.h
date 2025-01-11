#pragma once
#include "constants.cpp"

class Cell {
private:
    array <double, 9> fun_in;
    array <double, 9> fun_ex;
    array <double, 9> fun_eq;
    array<double, 2> velocity;
    double density;

public:
    Cell();
    Cell(array<double,9>);

    void set_fun(FunType, array<double,9>);
    void set_direct_fun(FunType, int, double);
    void set_density(double);
    void set_velocity(array<double,2>);
    void set_direct_velocity(int, double);

    const array<double,9>& get_fun(FunType) const;
    const array<double, 2>& get_velocity() const;
    double get_density();

    void calculate_velocity();
    void calculate_density();
    void calculate_fun_eq();
    void calculate_fun_ex();
};
