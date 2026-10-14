/******************************** Timer MACROS Configuration *********************************
 * @subject: Inteligencia Artificial
 * @file:lab5_ia.h
 * @hardware: ESP32 (PANTHER48)
 * @author: Paulina Cabriales y Esteban Cano
 * @date: 12/Octubre/2026
 * @overview: Archivo de librerias y definiciones del Laboratorio 5 de IA
 *********************************************************************************************/
#ifndef BSP_H
#define BSP_H

//Librerias
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_intr_alloc.h"
#include "esp_attr.h"
#include "esp32/rom/ets_sys.h"
#include <unistd.h> 
#include "driver/gpio.h" 	    	         	
#include "hal/gpio_types.h"
#include "esp_timer.h"
#include "esp_adc/adc_oneshot.h"

extern const char *TAG;

//Definiciones de pines
#define BOTON 18
#define LED 2
#define UMBRAL_ADC 2048
#define NUM_POTS_MAX 5
#define COMP_AND 1      // <-- nuevos
#define COMP_OR  2      // <-- nuevos

// Variables globales compartidas en el proyecto
extern volatile bool system_ON;
extern volatile bool state_led;

// Variables para la impresion
extern int n_entradas;           // Variable para guardar el numero de pots seleccionado
extern int compuerta;   // Variable para guardar la compuerte seleccionada

extern int adc_raw[NUM_POTS_MAX];
extern int X_inputs[NUM_POTS_MAX];

extern const adc_channel_t pot_channels[NUM_POTS_MAX];

// Prototipos de funciones
void gpio_init(void);
bool boton_presionado(void);
void adc_init(void);
void print_adc_values(int n_entradas);
void adc_read(void);
void adc_to_binary(void);
int leer_numero_consola(void);
int leer_compuerta(void);
void print_resultados(const double *X, const int *d, const double *w, double b,
                      int entradas, int patrones, int aciertos);
void hmi_enviar_config(int entradas, int compuerta, const double *w, double bias);
void hmi_enviar_estado(bool run);
void hmi_enviar_datos(int entradas, int salida);

#endif