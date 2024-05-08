// Librería para el control del LCD
#include <PCD8544.h>

// Pin de conexión para el dispositivo LCD
const int LCD_PIN = A4;

// Objeto para controlar el LCD
static PCD8544 lcd; 
void setup() {
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


void loop() {
  // Actualiza la pantalla LCD si el pin de control está en estado alto
  if (digitalRead(LCD_PIN) == HIGH) {
    // Alternar el estado de la pantalla LCD
    //LCD_control(pote, Output, Input);
  }
  }
  else {
    // Limpiar la pantalla LCD si el pin de control está en estado bajo
    lcd.clear();
  }
}
