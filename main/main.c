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

int N;          // Variable para ingresar el numero de pots

extern adc_oneshot_unit_handle_t adc1_handle;

void app_main(void) {
    gpio_init();
    N = leer_numero_consola();
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
           print_adc_values();
           adc_to_binary();
           vTaskDelay(pdMS_TO_TICKS(200));
        }
        vTaskDelay(pdMS_TO_TICKS(10));  
    }
}


