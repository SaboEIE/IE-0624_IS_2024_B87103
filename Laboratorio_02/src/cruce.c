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

/** @brief Rutina de interrupción externa.
 *  @details Esta función se activa cuando se produce una interrupción externa
 *  en el microcontrolador, en este caso específico, cuando ocurre una
 *  interrupción en el pin INT0. El objetivo de esta función es almacenar el
 *  una variable global llamada btn_pushed un 1, cuando el botón
 *  conectado al pin PD2 (pin 6) ha sido presionado. Lo anterior permite la
 *  activación de la máquina de estados más adelante.
 */
ISR(INT0_vect) { btn_pushed = 1; }

/** @brief Rutina de interrupción causada por el temporizador.
 *  @details Esta función permite controlar el tiempo del programa ya que tras 
 *  n comparaciones exitosos, suma una unidad a la varible global de segundos. 
 *  Lo anterior permite que la máquina de estados realice las transiciones en 
 *  los momentos adecuados. Asimismo, hace posible los parpadeos de los estados
 *  en los que esto es necesario.  
*/
ISR (TIMER0_COMPA_vect) {
    if (state == BLINK_CAR && (success_ctc == 30|| success_ctc == 60))
        PORTB ^= (1 << PB1);

    if (state == BLINK_WALKER && (success_ctc == 30 || success_ctc == 60))
        PORTB ^= (1 << PB3);

    if (success_ctc == 60){
        seconds++;
        success_ctc = 0; 
    }

    success_ctc++; 
}

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

/** @brief Función de configuración de registros del MCU.
 *  @details Esta función se encarga de configurar e inicializar los registros
 *  necesarios para permitir el funcionamiento esperado del microcontrolador.
 *  Al inicio de la función, se le da valor a alguna variables globales y se
 * habilitan las interrupciones globales haciendo el llamado de la función
 * sei(). Además, se configuran los pines del puerto B que se utilizarán, tanto
 * su estado como su modo. Lo siguiente es la configuración de la interrupción
 * externa INT0, la cual permite controla el cruce a través de un pulsador.
 */
void init()
{
    // Establecer el estado inicial de la FSM.
    state = GO_CAR;

    // Estado del botón: no ha sido presionado.
    btn_pushed = 0;

    // Inicializar la variable de segundos.
    seconds = 0;

    // Se hace el llamado de la función de habilitación de interrupciones.
    sei();

    // Se configuran los pines PB0, PB1, PB2, PB3 y PB4 como salidas.
    DDRB |= (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0);

    // Se inicia en el estado: semáforo vehicular en verde y peatonal en rojo.
    PORTB = (1 << PB4) | (0 << PB3) | (0 << PB2) | (0 << PB1) | (1 << PB0);

    // Se habilita la interrupción externa INT0.
    GIMSK |= (1 << INT0);

    // Configurar la interrupción externa INT0 para que se active en el flanco de subida
    MCUCR |= (1 << ISC00) | (1 << ISC01);

    // Se inicializa el valor del Timer0.
    TCNT0 = 0;

    // Valor de comparación
    OCR0A = 255;     

    // Se habilita el modo CTC 
    TCCR0A = (1 << WGM01) | (0 << WGM00);

    // Modo CTC y prescaler de 1024
    TCCR0B =  (0 << WGM02) | (1 << CS02) | (0 << CS01) | (1 << CS00);

    // Se habilita la interrupción por comparación A del Timer0.
    TIMSK |= (1 << OCIE0A); 
}