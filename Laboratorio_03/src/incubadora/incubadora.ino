/** @file incubadora.ino
 * @brief Programa que permite simular el funcionamiento de una incubadora utilizando un Arduino UNO.
 * @author Alexander Rojas Brenes - B86869
 * @author Kendall Saborío Picado - B87103
 * @date 08/04/2024
 */

// Librería para el control del LCD
#include <PCD8544.h>

// Librería para el control del PID
#include <PID_v1_bc.h>

// Inicialización de Pines/Variables.
// Pin de entrada para el potenciómetro
const int potPin = A0;

// Pin del LED azul
const int blue_led = 0;

// Pin del LED verde
const int green_led = 1; 

// Pin del LED rojo
const int red_led = 2; 

// Pin de conexión para el dispositivo LCD
const int LCD_PIN = A4;

// Pin de conexión para la comunicación serial
const int Serial_PIN = A5;

// Objeto para controlar el LCD
static PCD8544 lcd;

// Variables para el setpoint, la entrada y la salida del PID
double Setpoint, Input, Output; 

// Constantes para los coeficientes del PID
double consKp = 255, consKi = 0.0, consKd = 1;

// Variable para el valor convertido de la salida del PID
float output_coverted;

/**
 * @brief Configura los pines y los dispositivos necesarios al inicializar el 
 * programa.
 * @details Esta función se encarga de inicializar todo lo necesario para que 
 * el proyecto funcione correctamente.
 * Configura los pines de salida para los LEDs y de entrada para los 
 * dispositivos LCD y Serial, además inicia la comunicación con el LCD y 
 * configura el modo de operación del controlador PID.
 */
void setup() {
  // Pin de salida para el LED azul
  pinMode(blue_led, OUTPUT);

  // Pin de salida para el LED verde
  pinMode(green_led, OUTPUT); 

  // Pin de salida para el LED rojo 
  pinMode(red_led, OUTPUT);

  // Pin de entrada para el dispositivo LCD
  pinMode(LCD_PIN, INPUT);

  // Pin de entrada para la comunicación serial
  pinMode(Serial_PIN, INPUT);

  // Inicia la comunicación con el LCD con una resolución predeterminada de 84x48 píxeles
  lcd.begin();

  // Configura el modo de operación del controlador PID como AUTOMÁTICO
  myPID.SetMode(AUTOMATIC);

}
/**
 * @brief Controla la visualización en un LCD de las temperaturas operativa, de salida y la salida del controlador PID.
 * @details Esta función actualiza la pantalla LCD con las temperaturas operativa, de salida y la salida del controlador PID (Señal de control).
 * @param op_temp Temperatura operativa actual (en grados Celsius).
 * @param PID Salida del controlador PID.
 * @param out_temp Temperatura de salida actual (en grados Celsius).
 */
void LCD_control(double op_temp, double PID, double out_temp) {
  // Escribe en la primera línea 
  lcd.setCursor(0 , 0); 
  lcd.print(String("Op tmp: ") + String(op_temp));

  // Escribe en la segunda línea 
  lcd.setCursor(0 , 1); 
  lcd.print(String("PID: ") + String(PID));

  // Escribe en la tercera línea 
  lcd.setCursor(0 , 2); 
  lcd.print(String("Out tmp: ") + String(out_temp));
}

/**
 * @brief Controla los LEDs en función de la temperatura externa.
 * @details Esta función controla el encendido y apagado de los LEDs en base a la temperatura de salida
 * medida en la planta.
 * @param out_temp Temperatura de salida medida (en grados Celsius).
 */
void LED_control(double out_temp) {
  // Control de los LEDs en función de la temperatura sensada
  if (out_temp < 30.00) {
    // Enciende el LED azul
    digitalWrite(blue_led, HIGH);
    // Apaga el LED verde
    digitalWrite(green_led, LOW);
    // Apaga el LED rojo
    digitalWrite(red_led, LOW);
  }

  if (out_temp >= 30.00 && out_temp <= 42.00) {
    // Apaga el LED azul
    digitalWrite(blue_led, LOW);
    // Enciende el LED verde
    digitalWrite(green_led, HIGH);
    // Apaga el LED rojo
    digitalWrite(red_led, LOW);
  }
  
  if (out_temp > 42.00) {
    // Apaga el LED azul
    digitalWrite(blue_led, LOW); 
    // Apaga el LED verde
    digitalWrite(green_led, LOW);
    // Enciende el LED rojo
    digitalWrite(red_led, HIGH);
  }
}

/**
 * @brief Simula el comportamiento térmico de un bloque de aluminio con calentamiento y enfriamiento pasivo.
 * @details Esta función calcula la temperatura del bloque de aluminio en función del tiempo, considerando la transferencia de calor
 * desde un calentador y el enfriamiento por convección al ambiente.
 * @param Q Cantidad de calor suministrado al bloque de aluminio (en Julios por segundo).
 * @return T La temperatura actual del bloque de aluminio (en grados Celsius).
 */
float simPlanta(float Q){
  // simula un bloque de aluminio de 1x1x2cm con un calentador y con enfreamiento pasivo
  float h = 5; // W/m2K coeficiente de conveccion termica para el Aluminio
  float Cps = 0.89; // J/gC
  float area = 1e-4; // m2 area por conveccion
  float masa = 10 ; // g
  float Tamb = 25; // Temperatura ambiente en C
  static float T = Tamb; // Temperatura en C
  static uint32_t last = 0;
  uint32_t interval = 100; // ms
  
  if ( millis () - last >= interval) {
    last += interval;
    // 0−transferencia de calor
    T = T + Q * interval / 1000 / masa / Cps - (T - Tamb) *   area * h;
  }

  return T;
}

/** @brief Función que imprime valores en el puerto serial. 
 *  @details Esta función agrupa los valores de la temperatura de operación, la 
 *  señal de control y la señal de salida de la planta, de forma que estos 
 *  puedan ser captados y utilizados en futuro procesamiento. 
 *  @param op_temp valor de tipo double que representa el setpoint. 
 *  @param PID valor de tipo double que representa la señal de control. 
 *  @param out_temp valor de tipo double que representa la señal de salida. 
*/
void serial_com(double op_temp, double PID, double out_temp){
  Serial.println(String(op_temp) + String(", ") + String(PID) + String(", ") + String(out_temp));
}

/**
 * @brief Función principal del programa que se ejecuta repetidamente.
 * @details Esta función realiza las siguientes operaciones en un ciclo continuo:
 * 1. Lee el valor del potenciómetro.
 * 2. Mapea el valor leído a un rango de 20 a 80.
 * 3. Actualiza el valor de referencia del controlador PID (Setpoint).
 * 4. Calcula la salida del controlador PID.
 * 5. Convierte la salida en temperatura del controlador PID a watts.
 * 6. Simula el comportamiento térmico de la planta y obtiene la temperatura de salida.
 * 7. Controla los LEDs en función de la temperatura de salida.
 * 8. Actualiza la pantalla LCD si el pin de control está en estado alto.
 * 9. Inicia o detiene la comunicación serial si el pin de control está en estado alto.
 */
void loop() {
  // Lee el valor del potenciómetro
  int potValue = analogRead(potPin);

  // Mapea el valor leído a un rango de 20 a 80
  double potmappedValue = map(potValue, 0, 1023, 20, 80);

  // Actualiza el valor de referencia del controlador PID (Setpoint)
  Setpoint = potmappedValue;

  // Calcula la salida del controlador PID
  myPID.Compute();

  // Convierte la salida en temperatura del controlador PID a watts
  output_coverted = Output * 20.0 / 255;

  // Simula el comportamiento térmico de la planta y obtiene la temperatura de salida
  Input = simPlanta(output_coverted);

  // Función que controla los LED a partir de la temperatura sensada. 
  LED_control(Input);

  // Actualiza la pantalla LCD si el pin de control está en estado alto
  if (digitalRead(LCD_PIN) == HIGH) {
    // Alternar el estado de la pantalla LCD
    LCD_control(potmappedValue, Output, Input);
  }
  else {
    // Limpiar la pantalla LCD si el pin de control está en estado bajo
    lcd.clear();
  }

  // Inicia o detiene la comunicación serial si el pin de control está en estado alto
  if (digitalRead(Serial_PIN) == HIGH) {
    Serial.begin(9600);
    // Envia los datos a través del puerto serial
    serial_com(potmappedValue, Output, Input);
  }
  else {
    Serial.end();
  }
}
