/** @file semaforo.c
 * @brief Programa que permite simular el funcionamiento de un cruce peatonal con un ATtiny4313.
 * @author Alexander Rojas Brenes - B86869
 * @author Kendall Saborío Picado - B87103
 * @date 07/04/2024
 */

// Librerías necesarias para la ejecición
#include <avr/io.h>
#include <avr/interrupt.h>

// Definición de los states de la FSM
#define GO_CAR 0
#define BLINK_CAR 1
#define STOP_CAR 2
#define GO_WALKER 3
#define BLINK_WALKER 4
#define STOP_WALKER 5

// Definiciones de tiempos de semáforo
#define SAFE_TIME 1
#define BLINK_TIME 3
#define GO_WALKER_TIME 10
#define ENABLE_PUSH_TIME 10

// Estado actual: variable global
int state;

//
int seconds;
int success_ctc = 0;

// Estado de los botones del paso peatonal: variable global
int btn_pushed;

// Prototipos de funciones
void init();
void FSM();

/* DECLARACIÓN DE FUNCIONES */
// Prototipos de funciones
void init();
void FSM();

/** @brief Función principal.
 *  @details La función principal solamente hace el llamado de la función
 *  init() para configurar adecuadamente el ATtiny4313 y FSM() para iniciar con
 *  las transiciones de estado según sea el caso.
 */
int main(void)
{
    init();
    FSM();

    return 0;
}
