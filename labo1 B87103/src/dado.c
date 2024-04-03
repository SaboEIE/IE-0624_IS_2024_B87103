#include <pic14/pic12f683.h> // Incluye la biblioteca para el microcontrolador PIC12F683
#include <stdint.h>			 // Incluye la biblioteca para tipos de datos enteros definidos con anchos específicos
#include <stdio.h>			 // Incluye la biblioteca estándar de entrada y salida

typedef unsigned int word;				 // Define un alias 'word' para 'unsigned int'
word __at 0x2007 __CONFIG = (_WDTE_OFF); // Configura el registro de configuración en la dirección de memoria 0x2007 para desactivar el Watchdog Timer

void delay(unsigned int tiempo); // Prototipo de función para retrasar la ejecución por un tiempo determinado
int generate_random_number();	 // Prototipo de función para generar un número aleatorio
int super_seed;					 // Variable global para el estado del generador de números aleatorios

void main(void)
{
	TRISIO = 0b00100000;		// Configura los pines GPIO como salidas excepto el pin 5
	GPIO = 0x00;				// Establece todos los pines GPIO en bajo
	super_seed = 0xA3;			// Valor inicial arbitrario del LFSR
	unsigned int time = 200;	// Tiempo de retardo para el parpadeo de LED
	unsigned int num_aleatorio; // Variable para almacenar el número aleatorio generado

	// Bucle infinito
	while (1)
	{
		if (GP5 == 1) // Si se detecta una entrada alta en el pin GP5
		{
			num_aleatorio = generate_random_number(); // Genera un número aleatorio

			// Establece los pines GPIO según el número aleatorio generado y espera un tiempo
			if (num_aleatorio == 1)
			{
				GPIO = 0b00000100;
				delay(time);
				break;
			}
			if (num_aleatorio == 2)
			{
				GPIO = 0b00010000;
				delay(time);
				break;
			}
			if (num_aleatorio == 3)
			{
				GPIO = 0b00010100;
				delay(time);
				break;
			}
			if (num_aleatorio == 4)
			{
				GPIO = 0b00010001;
				delay(time);
				break;
			}
			if (num_aleatorio == 5)
			{
				GPIO = 0b00010101;
				delay(time);
				break;
			}
			if (num_aleatorio == 6)
			{
				GPIO = 0b00010011;
				delay(time);
				break;
			}
		}
		else
		{
			GPIO = 0b00000000; // Si no se detecta una entrada alta en el pin GP5, establece todos los pines GPIO en bajo
		}
		super_seed++; // Incrementa el estado del generador de números aleatorios
	}
}

void delay(unsigned int tiempo)
{
	unsigned int i;
	unsigned int j;

	// Bucle anidado para generar un retraso de tiempo
	for (i = 0; i < tiempo; i++)
		for (j = 0; j < 1275; j++)
			;
}

// Define las constantes para el LFSR
#define LFSR_TAPS 0x8005 // Retroalimentación para un LFSR de 16 bits, correspondiente a x^16 + x^15 + x^13 + x^4 + 1

// Función para generar un número aleatorio entre 1 y 6 utilizando un LFSR
int generate_random_number()
{
	// Realiza un paso en el LFSR
	uint16_t lsb = super_seed & 1;
	super_seed >>= 1;
	if (lsb)
	{
		super_seed ^= LFSR_TAPS;
	}

	// Calcula el número aleatorio entre 1 y 6
	int rand_num = ((super_seed >> 10) & 0x0007) + 1; // Toma 3 bits del estado del LFSR y suma 1
	return rand_num;								  // Retorna el número aleatorio generado
}