/******************************** Timer MACROS Configuration **********************************
 * @subject: Inteligencia Artificial
 * @file: main.c
 * @hardware: ESP32 (PANTHER48)
 * @author: Paulina Cabriales, Armando Garcia, Esteban Cano
 * @date: 12/Octubre/2026
 * @overview: Archivo main del Laboratorio 5 de IA
 *********************************************************************************************/
#include "BSP.h"
#include "PERCEPTRON.h"

volatile bool system_ON = false;
volatile bool state_led = false;

volatile bool btn_state = true;
volatile bool btn_state_old;

int n_entradas;     // Numero de pots que usa el sistema (1 a NUM_POTS_MAX)
int compuerta;      // 1 = AND, 2 = OR

extern adc_oneshot_unit_handle_t adc1_handle;

void app_main(void) {
    gpio_init();

    n_entradas = leer_numero_consola();
    ESP_LOGI(TAG, "Se seleccionaron %d pots", n_entradas);

    compuerta = leer_compuerta();
    ESP_LOGI(TAG, "Compuerta = %s", (compuerta == COMP_AND) ? "AND" : "OR");

    adc_init();

    // ===== BLOQUE DE PRUEBA (temporal): tabla de verdad =====
    int patrones = 1 << n_entradas;                              // 2^N combinaciones
    double *X = malloc(patrones * n_entradas * sizeof(double));  // memoria dinamica
    int *d = malloc(patrones * sizeof(int));

    if (X == NULL || d == NULL) {
        ESP_LOGE(TAG, "No hay memoria");
    }
    else {
        generar_patrones(X, n_entradas, patrones);
        generar_tabla_verdad(d, patrones, compuerta);
        print_tabla_verdad(X, d, n_entradas, patrones);
        free(X);
        free(d);
    }
    // ========================================================

    btn_state_old = gpio_get_level(BOTON);

    while(true) {
        btn_state = gpio_get_level(BOTON);

        if(boton_presionado()) {
            system_ON = !system_ON; 
            state_led = system_ON; 

            if(system_ON) {
                gpio_set_level(LED, true);
                ESP_LOGI(TAG, "MODO RUN - Sistema Activado");
            }
            else {
                gpio_set_level(LED, false);
                ESP_LOGI(TAG, "MODO OFF - Sistema Desactivado");
            }
        }
        btn_state_old = btn_state;

        if(system_ON) {
           adc_read();
           print_adc_values(n_entradas);
           adc_to_binary();
           vTaskDelay(pdMS_TO_TICKS(200));
        }
        vTaskDelay(pdMS_TO_TICKS(10));  
    }
}