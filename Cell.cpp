#include"Cell.h"

Cell::Cell() : fun_in(EMPTY), fun_ex(EMPTY), fun_eq(EMPTY), velocity({ 0,0 }), density(0) {}

Cell::Cell(array<double, 9> i) : fun_in(i), fun_ex(EMPTY), fun_eq(EMPTY), velocity({ 0,0 }), density(0){
    calculate_density();
    calculate_velocity();
}


void Cell::set_fun(FunType f, array<double,9> i) {
    if(f == FUN_IN) fun_in = i;
    else if (f == FUN_EX) fun_ex = i;
    else if (f == FUN_EQ) fun_eq = i;
}

void Cell::set_direct_fun(FunType f, int index, double num) {
    if (f == FUN_IN)fun_in[index] = num;
    else if (f == FUN_EX) fun_ex[index] = num;
    else if (f == FUN_EQ) fun_eq[index] = num;
}

void Cell::set_velocity(array<double, 2> v) {
    velocity = v;}

void Cell::set_direct_velocity(int index, double num) {
    velocity[index] = num;
}


const array<double,9>& Cell::get_fun(FunType f) const { 
    if (f == FUN_IN) return fun_in;
    else if (f == FUN_EX) return fun_ex;
    else if (f == FUN_EQ) return fun_eq;;
}

const array<double, 2>& Cell::get_velocity() const { return velocity; }

double Cell::get_density() { return density; }


void::Cell::calculate_velocity() {
    velocity[0] = (fun_in[1] + fun_in[5] + fun_in[8] - fun_in[2] - fun_in[6] - fun_in[7])/density;
    velocity[1] = (fun_in[3] + fun_in[5] + fun_in[6] - fun_in[4] - fun_in[7] - fun_in[8]) / density;
}

void Cell::calculate_density()
{
    density = 0.0;
    for (const auto& val : fun_in)
    {
        density += val;
    }

}

void Cell::calculate_fun_eq() { 
    
    for (int i = 0; i < 9; ++i) {
        double sc = (VELOCITY_FACTOR[i][0] * velocity[0]) + (VELOCITY_FACTOR[i][1] * velocity[1]);
        fun_eq[i] = (density * WEIGHT_FACTOR[i])*(1+ 3*sc + 4.5 * pow(sc,2) - 1.5 * (pow(velocity[0], 2) + pow(velocity[1], 2)));
    } 
}

void Cell::calculate_fun_ex() 
{ 
    calculate_fun_eq();
    for (int i = 0; i < 9; ++i) {
        fun_ex[i] = fun_in[i] + (1.0 / RELAXATION_TIME) * (fun_eq[i] - fun_in[i]);
     }
}
