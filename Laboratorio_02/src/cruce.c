/** @file semaforo.c
 * @brief Programa que permite simular el funcionamiento de un cruce peatonal con un ATtiny4313.
 * @author Alexander Rojas Brenes - B86869
 * @author Kendall Saborío Picado - B87103
 * @date 07/04/2024
 */

/*Librerías*/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/* DECLARACIÓN DE FUNCIONES */
// Prototipos de funciones
void setup();
void FSM();

void main(void)
{
    setup(); // Configuración.
    sei();   // Habilitación interrupciones.
    FSM();   // Ejecución de la simulación.
}
