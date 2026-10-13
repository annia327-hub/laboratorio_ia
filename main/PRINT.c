/*********************************************************************************************
 * @subject: Inteligencia Artificial
 * @file: print.c
 * @hardware: ESP32 (PANTHER48)
 * @author: Paulina Cabriales, Armando Garcia, Esteban Cano
 * @overview: Módulo de consola para impresión de datos del sistema (PRINT.c)
 *********************************************************************************************/
#include "BSP.h"

const char *TAG_PRINT = "PRINT";

void print_adc_values(void)
{
    ESP_LOGI(TAG_PRINT, "Pots -> P1: %4d | P2: %4d | P3: %4d | P4: %4d | P5: %4d", 
             X_inputs[0], X_inputs[1], X_inputs[2], X_inputs[3], X_inputs[4]);
}