/*********************************************************************************************
 * @subject: Inteligencia Artificial
 * @file: print.c
 * @hardware: ESP32 (PANTHER48)
 * @author: Paulina Cabriales, Armando Garcia, Esteban Cano
 * @overview: Módulo de consola para impresión de datos del sistema (PRINT.c)
 *********************************************************************************************/
#include "BSP.h"

const char *TAG_PRINT = "PRINT";

void print_adc_values(int N)
{
    // Rutina de impresion de valores del adc dependiendo del numero de pots
    printf("Pots ->  ");
    for (int i = 0; i < N; i++)
    {
        printf( "P%d: %d  ", i+1, X_inputs[i]);
    }
    puts("");
}

int leer_numero_consola(void) {
    ESP_LOGI(TAG_PRINT, " Ingresa el numero de pots: (1-%d): ", NUM_POTS_MAX);

    while (true) {
        int c = getchar();              // intenta leer una tecla

        if (c == EOF) {                 // nadie ha escrito nada todavia
            vTaskDelay(pdMS_TO_TICKS(50));
            continue;                   // vuelve a intentar
        }

        if (c >= '1' && c <= '0' + NUM_POTS_MAX) {   // digito entre 1 y NUM_POTS
            return c - '0';             // '3' (texto) -> 3 (numero)
        }
        // cualquier otra tecla (Enter, letras, 0, 7...) se ignora
    }
}

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