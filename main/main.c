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

double *w;          // Pesos del perceptron (N valores)
double b;           // Bias del perceptron

extern adc_oneshot_unit_handle_t adc1_handle;

void app_main(void) {
    gpio_init();

    n_entradas = leer_numero_consola();
    ESP_LOGI(TAG, "Se seleccionaron %d pots", n_entradas);

    compuerta = leer_compuerta();
    ESP_LOGI(TAG, "Compuerta = %s", (compuerta == COMP_AND) ? "AND" : "OR");

    adc_init();

    // ===== ENTRENAMIENTO DEL PERCEPTRON (una sola vez) =====
    int patrones = 1 << n_entradas;                              // 2^N combinaciones
    double *X = malloc(patrones * n_entradas * sizeof(double));  // tabla de entradas
    int *d = malloc(patrones * sizeof(int));                     // salida deseada
    w = malloc(n_entradas * sizeof(double));                     // pesos

    if (X == NULL || d == NULL || w == NULL) {
        ESP_LOGE(TAG, "No hay memoria");
        return;
    }

    generar_patrones(X, n_entradas, patrones);
    generar_tabla_verdad(d, patrones, compuerta);
    int aciertos = entrenar_perceptron(X, d, n_entradas, patrones, w, &b);
    print_resultados(X, d, w, b, n_entradas, patrones, aciertos);
    free(X);
    free(d);
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
                hmi_enviar_config(n_entradas, compuerta, w, b); // Manda la configuracion de HMI
            }
            else {
                gpio_set_level(LED, false);
                ESP_LOGI(TAG, "MODO OFF - Sistema Desactivado");
            }
            hmi_enviar_estado(system_ON);                         // HMI: manda RUN u OFF
        }
        btn_state_old = btn_state;

        if(system_ON) {
           adc_read();
           adc_to_binary();
           print_adc_values(n_entradas);

           double x[NUM_POTS_MAX];
           for (int j = 0; j < n_entradas; j++) {
               x[j] = X_inputs[j];
           }
           int salida = (producto_punto(w, x, n_entradas) + b >= 0.0) ? 1 : 0;
           ESP_LOGI(TAG, "Salida del perceptron = %d", salida);
           hmi_enviar_datos(n_entradas, salida);                  // HMI: manda pots y salida

           vTaskDelay(pdMS_TO_TICKS(200));
        }
        vTaskDelay(pdMS_TO_TICKS(10));  
    }
}