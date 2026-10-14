#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <stdlib.h>
#include <math.h>
#include "BSP.h"

#define MAX_EPOCAS 5000
#define TOLERANCIA 1e-12
#define PACIENCIA 20

void generar_patrones(double *X, int entradas, int patrones);
void generar_tabla_verdad(int *d_binaria, int patrones, int compuerta);
double producto_punto(const double *w, const double *x, int entradas);
int contar_aciertos(const double *X, const int *d_binaria,
                    const double *w, double b,
                    int entradas, int patrones);
void entrenar_lms(const double *X, const double *d,
                  const int *d_binaria,
                  int entradas, int patrones,
                  int clase_minoritaria, double factor_minoritario,
                  double eta, double *w, double *b,
                  double *mse_final, int *epocas_realizadas,
                  int *convergio, int *divergio);
int entrenar_perceptron(const double *X, const int *d_binaria,
                        int entradas, int patrones,
                        double *mejor_w, double *mejor_b);

#endif