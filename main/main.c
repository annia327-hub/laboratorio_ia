/******************************** Timer MACROS Configuration **********************************
 * @subject: Inteligencia Artificial
 * @file: main.c
 * @hardware: ESP32 (PANTHER48)
 * @author: Paulina Cabriales, Armando Garcia, Esteban Cano
 * @date: 12/Octubre/2026
 * @overview: Archivo main del Laboratorio 5 de IA
 *********************************************************************************************/
#include "BSP.h"

volatile bool system_ON = false;
volatile bool state_led = false;

volatile bool btn_state = true;
volatile bool btn_state_old;

int N;           // Variable para guardar el numero de pots seleccionado
int compuerta;   // Variable para guardar la compuerte seleccionada

extern adc_oneshot_unit_handle_t adc1_handle;

void app_main(void) {
    gpio_init();

    N = leer_numero_consola();
    ESP_LOGI(TAG, "Se seleccionaron %d pots", N);

    compuerta = leer_compuerta();
    if(compuerta == 1)
        ESP_LOGI(TAG, "Se selecciono la compuerta AND");
    else
        ESP_LOGI(TAG, "Se selecciono la compuerta OR");

    adc_init();

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
           print_adc_values(N);
           adc_to_binary();
           vTaskDelay(pdMS_TO_TICKS(200));
        }
        vTaskDelay(pdMS_TO_TICKS(10));  
    }
}


