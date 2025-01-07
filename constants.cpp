#pragma once

#include<iostream>
#include <array>
#include <random>
using namespace std;


constexpr array<double, 9> EMPTY = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 };
constexpr array<double, 9> WALL = { -1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0 };
constexpr array<double, 9> STARTING_STATE = { 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 };
constexpr array<double, 9> STARTING_STATE_2 = { 1.0 / 18.0, 1.0 / 18.0, 1.0 / 18.0, 1.0 / 18.0, 1.0 / 18.0, 1.0 / 18.0, 1.0 / 18.0, 1.0 / 18.0, 1.0 / 18.0 };
constexpr array<double, 9> WEIGHT_FACTOR = { 4.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0 };
constexpr array<array<double, 2>, 9> VELOCITY_FACTOR = { { {{0, 0}}, {{1, 0}}, {{-1, 0}}, {{0, 1}},{{0, -1}}, {{1, 1}}, {{-1, 1}}, {{-1, -1}}, {{1, -1}}} };
constexpr double RELAXATION_TIME = 0.75;

enum FunType { FUN_IN, FUN_EX, FUN_EQ };