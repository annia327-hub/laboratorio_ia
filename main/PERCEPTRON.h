#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "BSP.h"

void generar_patrones(double *X, int entradas, int patrones);
void generar_tabla_verdad(int *d_binaria, int patrones, int compuerta);
void print_tabla_verdad(const double *X, const int *d, int entradas, int patrones);

#endif