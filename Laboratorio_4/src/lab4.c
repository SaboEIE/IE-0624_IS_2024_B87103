/** @file lab4.c
 *  @brief Programa que permite simular el funcionamiento de un monitor de pendientes utilizando un STM32F429.
 *  @author Alexander Rojas Brenes - B86869
 *  @author Kendall Saborío Picado - B87103
 *  @date 25/05/2024
 */

#include <stdio.h>      // Biblioteca estándar de entrada y salida
#include <string.h>     // Biblioteca para manipulación de cadenas
#include <stdint.h>     // Biblioteca para definiciones de enteros con tamaño específico
#include <math.h>       // Biblioteca matemática estándar
#include "sdram.h"      // Biblioteca para manejar la SDRAM
#include "lcd-spi.h"    // Biblioteca para manejar la pantalla LCD vía SPI
#include "clock.h"      // Biblioteca para manejo del reloj del sistema
#include "console.h"    // Biblioteca para manejo de la consola
#include "gfx.h"        // Biblioteca para gráficos
#include "rcc.h"        // Biblioteca para el control del reloj y el sistema de reset
#include "adc.h"        // Biblioteca para manejo del convertidor analógico-digital
#include "gpio.h"       // Biblioteca para manejo de entradas y salidas generales
#include "spi.h"        // Biblioteca para manejar la comunicación SPI
#include "usart.h"      // Biblioteca para manejar la comunicación USART

// Definición de pi
#define PI 3.14159265

// Definiciones de constantes para el giroscopio
#define GYR_RNW         (1 << 7) // Bit para indicar lectura (Read not Write)
#define GYR_WHO_AM_I    0x0F     // Dirección de registro "Who Am I" del giroscopio
#define GYR_OUT_TEMP    0x26     // Dirección de registro de salida de temperatura
#define GYR_STATUS_REG  0x27     // Dirección de registro de estado
  

// Definiciones de constantes para el registro de control 1 del giroscopio
#define GYR_CTRL_REG1       0x20    // Dirección del registro de control 1
#define GYR_CTRL_REG1_PD    (1 << 3) // Bit para activar el modo de encendido (Power Down)
#define GYR_CTRL_REG1_XEN   (1 << 1) // Bit para habilitar el eje X
#define GYR_CTRL_REG1_YEN   (1 << 0) // Bit para habilitar el eje Y
#define GYR_CTRL_REG1_ZEN   (1 << 2) // Bit para habilitar el eje Z
#define GYR_CTRL_REG1_BW_SHIFT 4     // Desplazamiento de bits para el ancho de banda

// Definiciones de constantes para el registro de control 4 del giroscopio
#define GYR_CTRL_REG4       0x23    // Dirección del registro de control 4
#define GYR_CTRL_REG4_FS_SHIFT 4    // Desplazamiento de bits para la configuración de la escala completa

/**
 * @brief Configura el bus SPI (Interfaz Periférica Serial).
 * 
 * Esta función habilita los relojes para el bus SPI5 y los puertos GPIOC y GPIOF.
 * Configura el pin GPIOC1 como salida para el control del chip de esclavo.
 * Configura los pines GPIOF7, GPIOF8 y GPIOF9 para sus funciones alternas correspondientes (probablemente SCK, MISO, MOSI de SPI).
 * Configura los parámetros de SPI5, como modo maestro, velocidad de baudios, polaridad del reloj, fase del reloj, 
 * modo full duplex, modo unidireccional, control de esclavo mediante software, orden de transmisión de bits y configuraciones adicionales.
 * Finalmente, inicializa el giroscopio escribiendo en sus registros de control.
 */
static void spi_setup(void)
{
    rcc_periph_clock_enable(RCC_SPI5);  // Habilita el reloj para SPI5
    rcc_periph_clock_enable(RCC_GPIOC); // Habilita el reloj para el puerto GPIOC
    rcc_periph_clock_enable(RCC_GPIOF); // Habilita el reloj para el puerto GPIOF

    // Configurar el pin GPIOC1 como salida para el control del chip de esclavo
    gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO1);
    gpio_set(GPIOC, GPIO1); // Establecer el pin GPIOC1 en alto

    // Configurar los pines GPIOF7, GPIOF8 y GPIOF9 para funciones alternas correspondientes a SPI5
    gpio_mode_setup(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE,GPIO7 | GPIO8 | GPIO9);   
    gpio_set_af(GPIOF, GPIO_AF5, GPIO7 | GPIO8 | GPIO9);

    // Configuración de SPI5
    spi_set_master_mode(SPI5);                  // Establece SPI5 en modo maestro
    spi_set_baudrate_prescaler(SPI5, SPI_CR1_BR_FPCLK_DIV_64);  // Configura la velocidad de baudios de SPI5
    spi_set_clock_polarity_0(SPI5);             // Configura la polaridad del reloj a 0
    spi_set_clock_phase_0(SPI5);                // Configura la fase del reloj a 0
    spi_set_full_duplex_mode(SPI5);             // Establece SPI5 en modo full duplex
    spi_set_unidirectional_mode(SPI5);          // Establece SPI5 en modo unidireccional (pero con 3 cables)
    spi_enable_software_slave_management(SPI5); // Habilita la gestión de esclavo por software
    spi_send_msb_first(SPI5);                   // Configurar la transmisión de bits de MSB primero
    spi_set_nss_high(SPI5);                     // Establece el pin NSS (Chip Select) en alto
    SPI_I2SCFGR(SPI5) &= ~SPI_I2SCFGR_I2SMOD;   // Configuración adicional para SPI5
    spi_enable(SPI5);                           // Habilita SPI5

    // Inicializar el giroscopio escribiendo en sus registros de control
    write_reg(GYR_CTRL_REG1, GYR_CTRL_REG1_PD | GYR_CTRL_REG1_XEN | GYR_CTRL_REG1_YEN | GYR_CTRL_REG1_ZEN | (3 << GYR_CTRL_REG1_BW_SHIFT));
    write_reg(GYR_CTRL_REG4, (1 << GYR_CTRL_REG4_FS_SHIFT));
}

/**
 * @brief Configura la USART (Universal Synchronous/Asynchronous Receiver/Transmitter).
 * 
 * Esta función configura la USART1 para la comunicación serie asíncrona.
 * Configura el pin GPIOA9 para su uso como función alternativa correspondiente a USART1.
 * Establece la velocidad de baudios en 115200, el número de bits de datos en 8, 
 * el número de bits de parada en 1, sin paridad, sin control de flujo, y en modo de transmisión solamente.
 * Finalmente, habilita la USART1.
 */
static void usart_setup(void)
{
    // Configura el pin GPIO9 de GPIOA para transmitir en USART1
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);	
    gpio_set_af(GPIOA, GPIO_AF7, GPIO9);
	
    // Configuración de USART1
    usart_set_baudrate(USART1, 115200);           // Configurar la velocidad de baudios en 115200
    usart_set_databits(USART1, 8);                // Configurar el número de bits de datos en 8
    usart_set_stopbits(USART1, USART_STOPBITS_1); // Configurar el número de bits de parada en 1
    usart_set_mode(USART1, USART_MODE_TX);        // Configurar la USART1 en modo de transmisión solamente
    usart_set_parity(USART1, USART_PARITY_NONE);  // Configurar la paridad como ninguna
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE); // Configurar el control de flujo como ninguno
    usart_enable(USART1);                         // Habilita USART1
}