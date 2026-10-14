/*********************************************************************************************
 * @subject: Inteligencia Artificial
 * @file: print.c
 * @hardware: ESP32 (PANTHER48)
 * @author: Paulina Cabriales, Armando Garcia, Esteban Cano
 * @overview: Módulo de consola para impresión de datos del sistema (PRINT.c)
 *********************************************************************************************/
#include "BSP.h"
#include "PERCEPTRON.h"

const char *TAG_PRINT = "PRINT";

// Imprime en un solo renglon el valor (0 o 1) de cada pot en uso
void print_adc_values(int n_entradas) {
    printf("Pots -> ");
    for (int i = 0; i < n_entradas; i++) {
        printf("P%d: %d | ", i + 1, X_inputs[i]);
    }
    printf("\n");
}

// Pide por consola cuantos pots se van a usar (1 a NUM_POTS_MAX)
int leer_numero_consola(void) {
    ESP_LOGI(TAG_PRINT, " Ingresa el numero de pots: (1-%d): ", NUM_POTS_MAX);

    while (true) {
        int c = getchar();              // intenta leer una tecla

        if (c == EOF) {                 // nadie ha escrito nada todavia
            vTaskDelay(pdMS_TO_TICKS(50));
            continue;                   // vuelve a intentar
        }

        if (c >= '1' && c <= '0' + NUM_POTS_MAX) {   // digito entre 1 y NUM_POTS_MAX
            return c - '0';             // '3' (texto) -> 3 (numero)
        }
        // cualquier otra tecla (Enter, letras, 0, 7...) se ignora
    }
}

// Pide por consola la compuerta: 1 = AND, 2 = OR
int leer_compuerta(void) {
    ESP_LOGI(TAG_PRINT, "Elige la compuerta: 1 = AND, 2 = OR");

    while (true) {
        int c = getchar();              // intenta leer una tecla

        if (c == EOF) {                 // nadie ha escrito nada todavia
            vTaskDelay(pdMS_TO_TICKS(50));
            continue;                   // vuelve a intentar
        }

        if (c == '1' || c == '2') {     // solo acepta 1 o 2
            return c - '0';
        }
        // cualquier otra tecla se ignora
    }
}

// Imprime pesos, bias y la salida del perceptron para cada patron
void print_resultados(const double *X, const int *d, const double *w, double b,
                      int entradas, int patrones, int aciertos) {
    printf("\nPesos finales:\n");
    for (int j = 0; j < entradas; j++) {
        printf("w%d = %.6f\n", j + 1, w[j]);
    }
    printf("Bias = %.6f\n", b);

    printf("\nResultados por patron:\n");
    for (int i = 0; i < patrones; i++) {
        double y = producto_punto(w, &X[i * entradas], entradas) + b;
        int salida = (y >= 0.0) ? 1 : 0;

        printf("[");
        for (int j = 0; j < entradas; j++) {
            printf("%.0f ", X[i * entradas + j]);
        }
        printf("] -> salida: %d | deseada: %d | lineal: % .6f\n", salida, d[i], y);
    }
    printf("\nClasificaciones correctas: %d/%d\n", aciertos, patrones);
}

// Interfaz grafica
// CFG,n,compuerta,w1..wn,bias
void hmi_enviar_config(int entradas, int compuerta, const double *w, double bias) {
    printf("CFG,%d,%d", entradas, compuerta);
    for (int j = 0; j < entradas; j++)
        printf(",%.6f", w[j]);

    printf(",%.6f\n", bias);
}

// STATE,RUN  o  STATE,OFF
void hmi_enviar_estado(bool run) {
    printf("STATE,%s\n", run ? "RUN" : "OFF");
}

// DATA,raw1..rawN,salida
void hmi_enviar_datos(int entradas, int salida) {
    printf("DATA");
    for (int j = 0; j < entradas; j++)
        printf(",%d", adc_raw[j]);

    printf(",%d\n", salida);
}