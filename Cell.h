#pragma once
#include<iostream>
#include <array>
#include <random>

using namespace std;

constexpr array<double, 9> EMPTY = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 };
constexpr array<double, 9> WALL = { -1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0 };
constexpr array<double, 9> STARTING_STATE = { 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0/9.0 };
constexpr array<double, 9> WEIGHT_FACTOR = { 4.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0 }; 
constexpr array<array<double, 2>, 9> VELOCITY_FACTOR = { { {{0, 0}}, {{1, 0}}, {{-1, 0}}, {{0, 1}},{{0, -1}}, {{1, 1}}, {{-1, 1}}, {{-1, -1}}, {{1, -1}}} };
constexpr double RELAXATION_TIME = 0.75;

enum FunType{FUN_IN,FUN_EX, FUN_EQ};

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
