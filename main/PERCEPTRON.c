#include "PERCEPTRON.h"

// Llena X con todas las combinaciones de 0 y 1 (cuenta en binario)
void generar_patrones(double *X, int n_entradas, int patrones) {
    for (int i = 0; i < patrones; i++) {
        for (int j = 0; j < n_entradas; j++) {
            int desplazamiento = n_entradas - 1 - j;
            X[i * n_entradas + j] = (double)((i >> desplazamiento) & 1UL);
        }
    }
}

// Llena la salida deseada de cada patron segun la compuerta
void generar_tabla_verdad(int *d_binaria, int patrones, int compuerta) {
    for (int i = 0; i < patrones; i++) {
        if (compuerta == COMP_AND) {
            d_binaria[i] = (i == patrones - 1) ? 1 : 0;   // solo el ultimo (todos 1) da 1
        } else {
            d_binaria[i] = (i == 0) ? 0 : 1;              // solo el primero (todos 0) da 0
        }
    }
}