#include "PERCEPTRON.h"

void generar_patrones(double *X, int entradas, int patrones) {
    for (int i = 0; i < patrones; i++) {
        for (int j = 0; j < entradas; j++) {
            int desplazamiento = entradas - 1 - j;
            X[i * entradas + j] = (double)((i >> desplazamiento) & 1);
        }
    }
}

void generar_tabla_verdad(int *d_binaria, int patrones, int compuerta) {
    for (int i = 0; i < patrones; i++) {
        if (compuerta == COMP_AND) {
            d_binaria[i] = (i == patrones - 1) ? 1 : 0;
        } else {
            d_binaria[i] = (i == 0) ? 0 : 1;
        }
    }
}

double producto_punto(const double *w, const double *x, int entradas)
{
    int j;
    double suma = 0.0;

    for (j = 0; j < entradas; j++)
        suma += w[j] * x[j];

    return suma;
}

int contar_aciertos(const double *X, const int *d_binaria,
                    const double *w, double b,
                    int entradas, int patrones)
{
    int i;
    int aciertos = 0;

    for (i = 0; i < patrones; i++) {
        double y = producto_punto(w, &X[i * entradas], entradas) + b;
        int salida = (y >= 0.0) ? 1 : 0;

        if (salida == d_binaria[i])
            aciertos++;
    }

    return aciertos;
}

/*
   Entrena LMS por lote. La clase minoritaria recibe una ponderacion
   ajustable. Esto ayuda con AND y OR cuando hay muchas entradas.
*/
void entrenar_lms(const double *X, const double *d,
                  const int *d_binaria,
                  int entradas, int patrones,
                  int clase_minoritaria, double factor_minoritario,
                  double eta, double *w, double *b,
                  double *mse_final, int *epocas_realizadas,
                  int *convergio, int *divergio)
{
    int i;
    int j;
    int epoca;
    int contador_estable = 0;
    double mse_anterior = 0.0;
    double *gradiente_w;
    int cantidad_minoritaria = 0;
    int cantidad_mayoritaria;
    double suma_pesos;
    double escala;

    gradiente_w = (double *)calloc((size_t)entradas, sizeof(double));

    if (gradiente_w == NULL) {
        ESP_LOGE(TAG, "Error al reservar memoria para el gradiente.");
        *convergio = 0;
        *divergio = 1;
        *mse_final = 0.0;
        *epocas_realizadas = 0;
        return;
    }

    for (i = 0; i < patrones; i++) {
        if (d_binaria[i] == clase_minoritaria)
            cantidad_minoritaria++;
    }

    cantidad_mayoritaria = patrones - cantidad_minoritaria;

    if (cantidad_minoritaria == 0 || cantidad_mayoritaria == 0) {
        factor_minoritario = 1.0;
        escala = 1.0;
    }
    else {
        suma_pesos = factor_minoritario * cantidad_minoritaria
                    + cantidad_mayoritaria;
        escala = (double)patrones / suma_pesos;
    }

    for (j = 0; j < entradas; j++)
        w[j] = 0.0;

    *b = 0.0;
    *convergio = 0;
    *divergio = 0;
    *mse_final = 0.0;
    *epocas_realizadas = 0;

    for (epoca = 0; epoca < MAX_EPOCAS; epoca++) {
        double gradiente_b = 0.0;
        double error_epoca = 0.0;

        for (j = 0; j < entradas; j++)
            gradiente_w[j] = 0.0;

        for (i = 0; i < patrones; i++) {
            double y;
            double e;
            double peso_muestra;

            if (cantidad_minoritaria == 0 || cantidad_mayoritaria == 0)
                peso_muestra = 1.0;
            else if (d_binaria[i] == clase_minoritaria)
                peso_muestra = factor_minoritario * escala;
            else
                peso_muestra = escala;

            y = producto_punto(w, &X[i * entradas], entradas) + *b;
            e = d[i] - y;

            for (j = 0; j < entradas; j++) {
                gradiente_w[j] += peso_muestra * e
                                  * X[i * entradas + j];
            }

            gradiente_b += peso_muestra * e;
            error_epoca += peso_muestra * e * e;
        }

        for (j = 0; j < entradas; j++)
            w[j] += eta * gradiente_w[j] / (double)patrones;

        *b += eta * gradiente_b / (double)patrones;
        *mse_final = error_epoca / (double)patrones;
        *epocas_realizadas = epoca + 1;

        if (*mse_final != *mse_final || *mse_final > 1e100) {
            *divergio = 1;
            break;
        }

        if (epoca > 0) {
            double cambio = fabs(*mse_final - mse_anterior);

            if (cambio < TOLERANCIA)
                contador_estable++;
            else
                contador_estable = 0;

            if (contador_estable >= PACIENCIA) {
                *convergio = 1;
                break;
            }
        }

        mse_anterior = *mse_final;
    }

    free(gradiente_w);
}

/*
   Esta funcion es la parte de main() del profe que prueba varios factores
   de ponderacion y se queda con el mejor. Deja los pesos en mejor_w
   (arreglo de N valores) y el bias en *mejor_b. Regresa los aciertos.
*/
int entrenar_perceptron(const double *X, const int *d_binaria,
                        int entradas, int patrones,
                        double *mejor_w, double *mejor_b)
{
    int i;
    int j;
    int cantidad_cero = 0;
    int cantidad_uno = 0;
    int clase_minoritaria;
    int mejor_aciertos = -1;
    int mejores_epocas = 0;
    int mejor_convergio = 0;
    int mejor_divergio = 0;
    double mejor_mse = 1e300;
    double mejor_factor = 1.0;
    double eta = 0.1;
    double *d;
    double *w;

    d = (double *)malloc((size_t)patrones * sizeof(double));
    w = (double *)malloc((size_t)entradas * sizeof(double));

    if (d == NULL || w == NULL) {
        ESP_LOGE(TAG, "No fue posible reservar la memoria requerida.");
        free(d);
        free(w);
        return -1;
    }

    for (i = 0; i < patrones; i++) {
        d[i] = (d_binaria[i] == 0) ? -1.0 : 1.0;

        if (d_binaria[i] == 0)
            cantidad_cero++;
        else
            cantidad_uno++;
    }

    if (cantidad_cero <= cantidad_uno)
        clase_minoritaria = 0;
    else
        clase_minoritaria = 1;

    /*
       Se prueban automaticamente varios niveles de ponderacion.
       Se conserva el modelo con mayor cantidad de aciertos.
    */
    {
        double factor = 1.0;
        double factor_maximo;
        int intento = 0;

        if (cantidad_cero == 0 || cantidad_uno == 0)
            factor_maximo = 1.0;
        else if (clase_minoritaria == 0)
            factor_maximo = 2.0 * cantidad_uno / (double)cantidad_cero;
        else
            factor_maximo = 2.0 * cantidad_cero / (double)cantidad_uno;

        while (factor <= factor_maximo + 1e-12) {
            double b_actual;
            double mse_actual;
            int epocas_actuales;
            int convergio_actual;
            int divergio_actual;
            int aciertos_actuales;

            entrenar_lms(
                X, d, d_binaria, entradas, patrones,
                clase_minoritaria, factor, eta,
                w, &b_actual, &mse_actual, &epocas_actuales,
                &convergio_actual, &divergio_actual
            );

            aciertos_actuales = contar_aciertos(
                X, d_binaria, w, b_actual, entradas, patrones
            );

            if (aciertos_actuales > mejor_aciertos
                || (aciertos_actuales == mejor_aciertos
                    && mse_actual < mejor_mse)) {
                mejor_aciertos = aciertos_actuales;
                mejor_mse = mse_actual;
                mejores_epocas = epocas_actuales;
                mejor_convergio = convergio_actual;
                mejor_divergio = divergio_actual;
                mejor_factor = factor;
                *mejor_b = b_actual;

                for (j = 0; j < entradas; j++)
                    mejor_w[j] = w[j];
            }

            intento++;

            if (factor_maximo == 1.0)
                break;

            factor *= 2.0;

            if (factor > factor_maximo && factor / 2.0 < factor_maximo)
                factor = factor_maximo;

            if (intento > 30)
                break;
        }
    }

    ESP_LOGI(TAG, "Entrenamiento: epocas=%d, MSE=%.6f, factor=%.4f, convergio=%d, divergio=%d",
             mejores_epocas, mejor_mse, mejor_factor, mejor_convergio, mejor_divergio);

    free(d);
    free(w);

    return mejor_aciertos;
}