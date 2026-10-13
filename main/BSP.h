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

//LIBRERIAS
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

//DEFINICIONES DE PINES
#define BOTON 18
#define LED 5
#define UMBRAL_ADC 2048
#define NUM_POTS_MAX 5

// VARIABLES GLOBALES COMPARTIDAS EN EL PROYECTO
extern volatile bool system_ON;
extern volatile bool state_led;

extern int N;

extern int adc_raw[N];
extern int X_inputs[N];

extern const adc_channel_t pot_channels[N];

// PROTOTIPOS DE FUNCIONES
void gpio_init(void);
bool boton_presionado(void);
void adc_init(void);
void print_adc_values(void);
void adc_read(void);
void adc_to_binary(void);
int leer_numero_consola(void);

#endif