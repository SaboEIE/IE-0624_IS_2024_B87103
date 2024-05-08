// Librería para el control del LCD
#include <PCD8544.h>

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

// Objeto para controlar el LCD
static PCD8544 lcd; 


void setup() {
  // Pin de salida para el LED azul
  pinMode(blue_led, OUTPUT);

  // Pin de salida para el LED verde
  pinMode(green_led, OUTPUT); 

  // Pin de salida para el LED rojo 
  pinMode(red_led, OUTPUT);

  // Pin de entrada para el dispositivo LCD
  pinMode(LCD_PIN, INPUT);

  // Inicia la comunicación con el LCD con una resolución predeterminada de 84x48 píxeles
  lcd.begin();

}

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


void loop() {
  // Lee el valor del potenciómetro
  int potValue = analogRead(potPin);

  // Mapea el valor leído a un rango de 20 a 80
  double potmappedValue = map(potValue, 0, 1023, 20, 80);

  // Actualiza el valor de referencia del controlador PID (Setpoint)
  Setpoint = potmappedValue;

  // Función que controla los LED a partir de la temperatura sensada. 
  LED_control(Input);

  // Actualiza la pantalla LCD si el pin de control está en estado alto
  if (digitalRead(LCD_PIN) == HIGH) {
    // Alternar el estado de la pantalla LCD
    //LCD_control(pote, Output, Input);
  }
  else {
    // Limpiar la pantalla LCD si el pin de control está en estado bajo
    lcd.clear();
  }
}
