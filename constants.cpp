#pragma once

#include <SFML/Window.hpp>
#include <GL/glew.h>
#include<iostream>
#include <thread>
#include <array>
#include <random>
using namespace std;


constexpr array<double, 9> EMPTY = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 };
constexpr array<double, 9> WALL = { -1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0 };
constexpr array<double, 9> WEIGHT_FACTOR = { 4.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0 };
constexpr array<array<double, 2>, 9> VELOCITY_FACTOR = { { {{0, 0}}, {{1, 0}}, {{-1, 0}}, {{0, 1}},{{0, -1}}, {{1, 1}}, {{-1, 1}}, {{-1, -1}}, {{1, -1}}} };
constexpr array<int, 9> OPPOSITE_DIRECTION = { 0, 2, 1, 4, 3, 6, 5, 8, 7 };
constexpr double RELAXATION_TIME = 1.0;

enum FunType { FUN_IN, FUN_EX, FUN_EQ };