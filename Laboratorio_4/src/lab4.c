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

// Definiciones de constantes para los registros de salida del giroscopio
#define GYR_OUT_X_L       0x28    // Dirección del registro de salida del eje X (bajo)
#define GYR_OUT_X_H       0x29    // Dirección del registro de salida del eje X (alto)
#define GYR_OUT_Y_L       0x2A    // Dirección del registro de salida del eje Y (bajo)
#define GYR_OUT_Y_H       0x2B    // Dirección del registro de salida del eje Y (alto)
#define GYR_OUT_Z_L       0x2C    // Dirección del registro de salida del eje Z (bajo)
#define GYR_OUT_Z_H       0x2D    // Dirección del registro de salida del eje Z (alto)

// Sensibilidad del giroscopio L3GD20 a 250 grados por segundo
#define L3GD20_SENSITIVITY_250DPS  (0.00875F) // Sensibilidad del giroscopio a 250 dps 

/**
 * @struct Gyro
 * @brief Estructura para almacenar los datos del giroscopio.
 */
typedef struct Gyro {
  int16_t x; /**< Valor del eje X del giroscopio */
  int16_t y; /**< Valor del eje Y del giroscopio */
  int16_t z; /**< Valor del eje Z del giroscopio */
  int16_t temp; /**< Valor de la temperatura del giroscopio */
} gyro;

int main(void) {
    gyro lectura; // Estructura para almacenar los datos del giroscopio
    
    init_system(); // Inicializar el sistema
    
    // Bucle principal
    while (1) {
        lectura = read_xyz_temp();
        gpio_set(GPIOC, GPIO1);     // Establecer el pin GPIOC1 
    }
    return 0;  // Retorna 0 (aunque en realidad nunca llegará a esta línea ya que el bucle anterior es infinito).
}

//------------------------ Funciones de configuración -----------------------//
/**
 * @brief Configura los pines GPIO necesarios.
 * 
 * Esta función habilita los relojes para los puertos GPIOA y GPIOG, y configura:
 * - GPIOA0 como entrada.
 * - GPIOG13 y GPIOG14 como salidas.
 */
static void gpio_setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOG);  // Habilitar el reloj para el puerto GPIOG
    rcc_periph_clock_enable(RCC_GPIOA);  // Habilitar el reloj para el puerto GPIOA

    // Configurar GPIOA0 como entrada
    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
    // Configurar GPIOG13 como salida
    gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
    // Configurar GPIOG14 como salida
    gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
}

/**
 * @brief Configura el ADC (Convertidor Analógico-Digital).
 * 
 * Esta función configura el ADC para utilizar el canal GPIO3 como entrada analógica.
 * Se apaga el ADC, se desactiva el modo de escaneo, se establece el tiempo de muestreo en todos los canales,
 * y luego se enciende el ADC.
 */
static void adc_setup(void)
{
    // Configurar GPIOA3 como entrada analógica 
    gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO3);
    // Apagar el ADC1
    adc_power_off(ADC1);
    // Desactivar el modo de escaneo del ADC1
    adc_disable_scan_mode(ADC1);
    // Establecer el tiempo de muestreo en todos los canales del ADC1 a 3 ciclos
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_3CYC);
    // Encender el ADC1
    adc_power_on(ADC1);
}

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

//------------------------------INICIALIZACIÓN GENERAL-----------------------//
/**
 * @brief Inicializa el sistema.
 * 
 * Esta función inicializa todos los periféricos y configuraciones necesarios para el sistema.
 * Configura la consola para la comunicación serie con una velocidad de baudios de 115200.
 * Configura el reloj del sistema.
 * Habilita los relojes para USART1 y ADC1.
 * Configura los pines GPIO necesarios.
 * Inicializa el ADC.
 * Inicializa la memoria SDRAM.
 * Configura la USART1 para la comunicación serie.
 * Configura el bus SPI.
 * Inicializa la pantalla LCD mediante SPI.
 * Inicializa el sistema gráfico.
 */
void init_system(void) {
    console_setup(115200); // Configurar la consola para la comunicación serie con una velocidad de baudios de 115200
    clock_setup(); // Configurar el reloj del sistema
    rcc_periph_clock_enable(RCC_USART1); // Habilitar el reloj para USART1
    rcc_periph_clock_enable(RCC_ADC1); // Habilitar el reloj para ADC1
    gpio_setup(); // Configurar los pines GPIO necesarios
    adc_setup(); // Inicializar el ADC
    sdram_init(); // Inicializar la memoria SDRAM
    usart_setup(); // Configurar USART1 para la comunicación serie
    spi_setup(); // Configurar el bus SPI
    lcd_spi_init(); // Inicializar la pantalla LCD mediante SPI
    gfx_init(lcd_draw_pixel, 240, 320); // Inicializar el sistema gráfico
}

//------------------------------------ SPI ----------------------------------//
/**
 * @brief Escribe un valor en un registro del dispositivo mediante SPI.
 * 
 * Esta función envía una secuencia de datos SPI para escribir un valor en un registro específico del dispositivo.
 * 
 * @param reg El registro al que se va a escribir el valor.
 * @param val El valor que se va a escribir en el registro.
 */
void write_reg(uint16_t reg, uint16_t val) {
    gpio_clear(GPIOC, GPIO1); // Bajar CS (Chip Select) para comenzar transacción
    spi_send(SPI5, reg);      // Enviar registro al giroscopio
    spi_read(SPI5);           // Leer respuesta del giroscopio
    spi_send(SPI5, val);      // Enviar valor al giroscopio
    spi_read(SPI5);           // Leer respuesta del giroscopio
    gpio_set(GPIOC, GPIO1);   // Subir CS para finalizar transacción
}

/**
 * @brief Lee un registro del dispositivo mediante SPI.
 * 
 * Esta función envía una secuencia de datos SPI para leer un registro específico del dispositivo.
 * 
 * @param command El comando para leer el registro.
 * @return El valor leído del registro.
 */
uint8_t read_reg(uint8_t command) {
    gpio_clear(GPIOC, GPIO1);          // Desactiva el pin GPIO1 de GPIOC (probablemente CS o Chip Select)
    spi_send(SPI5, command);           // Envia el comando por SPI5
    spi_read(SPI5);                    // Lee y descarta una respuesta desde SPI5
    spi_send(SPI5, 0);                 // Envía un dato de relleno (0) para iniciar la lectura del registro
    uint8_t result = spi_read(SPI5);   // Lee el resultado desde SPI5
    gpio_set(GPIOC, GPIO1);            // Activa el pin GPIO1 de GPIOC (termina la comunicación con el dispositivo)
    return result;                     // Devuelve el resultado leído
}

/**
 * @brief Lee los valores de los ejes X, Y, Z y la temperatura del giroscopio.
 * 
 * Esta función lee los valores de los ejes X, Y, Z y la temperatura del giroscopio 
 * mediante la comunicación SPI con el dispositivo.
 * 
 * @return Una estructura que contiene los valores de los ejes X, Y, Z y la temperatura leídos del giroscopio.
 */
gyro read_xyz_temp(void) {
    // Estructura para almacenar los valores de los ejes X, Y, Z y la temperatura
    gyro lectura;

    // Leer WHO_AM_I (registro de identificación) con el bit R/W en 1 (lectura)
    read_reg(GYR_WHO_AM_I | 0x80);

    // Leer STATUS_REG (registro de estado) con el bit R/W en 1 (lectura)            
    read_reg(GYR_STATUS_REG | GYR_RNW);              

    // Leer y calcular el valor del eje correspondiente, multiplicando por la sensibilidad correspondiente
    lectura.x = read_axis(GYR_OUT_X_L | GYR_RNW, GYR_OUT_X_H | GYR_RNW) * L3GD20_SENSITIVITY_250DPS;
    lectura.y = read_axis(GYR_OUT_Y_L | GYR_RNW, GYR_OUT_Y_H | GYR_RNW) * L3GD20_SENSITIVITY_250DPS;
    lectura.z = read_axis(GYR_OUT_Z_L | GYR_RNW, GYR_OUT_Z_H | GYR_RNW) * L3GD20_SENSITIVITY_250DPS;

    // Leer el valor de la temperatura
    lectura.temp = read_reg(GYR_OUT_TEMP | GYR_RNW);

    // Retornar la estructura con los valores leídos de los ejes X, Y, Z y la temperatura
    return lectura; 
}