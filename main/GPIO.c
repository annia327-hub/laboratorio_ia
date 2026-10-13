/******************************** Timer MACROS Configuration **********************************
 * @subject: Inteligencia Artificial
 * @file: GPIO.c
 * @hardware: ESP32 (PANTHER48)
 * @author: Paulina Cabriales, Armando Garcia, Esteban Cano
 * @date: 12/Octubre/2026
 * @overview: Archivo de funciones para uso de GPIO del Laboratorio 5 de IA
 *********************************************************************************************/

#include "BSP.h"

const char *TAG = "GPIO";

void gpio_init()
{
    gpio_reset_pin(LED);                     // Resetea el pin
    gpio_set_direction(LED,GPIO_MODE_OUTPUT);// Configura el pin como salida
    gpio_set_level(LED, false);       
    
    gpio_reset_pin(BOTON);                 // resetea el pin
	gpio_set_direction(BOTON, GPIO_MODE_INPUT);// Se configura como entrada
	gpio_set_pull_mode(BOTON, GPIO_PULLUP_ONLY);// Se activa resistencia pull-up
}

bool boton_presionado(void)
{
    static bool btn_state_old = true;
    bool btn_state = gpio_get_level(BOTON);

    bool presionado = (btn_state_old && !btn_state);

    btn_state_old = btn_state;
    return presionado;
}