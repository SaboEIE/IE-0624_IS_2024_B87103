/* Informacion de la cuenta de blink*/
#define BLYNK_TEMPLATE_ID "TMPL2bLaW_hSV"
#define BLYNK_TEMPLATE_NAME "ESP8266"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

char auth[ ] = "jyE8_LKpX_TwtKBJHrLlW870nAoHT2c3";
char ssid[ ] = "Jupiter";
char pass[ ] = ".jup1t3r.";

int pinValue_food = 0;    // Variable para almacenar el valor del pin virtual
int pinValue_water = 0;    // Variable para almacenar el valor del pin virtual

// Pin para controlar la bomba de agua
const int waterPump = D1; 

// Se definen los pines para los sensores ultrsónicos
// Pines para el primer sensor: Contenedor de agua
const int trigPin_us1 = D0;
const int echoPin_us1 = 3;

// Pines para el segundo sensor: Contenedor de comida
const int trigPin_us2 = D3;
const int echoPin_us2 = D4;

// Pines para el tercer sensor: Taza de agua *
const int trigPin_us3 = D5;
const int echoPin_us3 = D6;

// Pines para el cuarto sensor: Taza de comida
const int trigPin_us4 = 9;
const int echoPin_us4 = 10;

// Declarar la variable para el pin del servo
const int pinServo = D2; 

// Crear un objeto de la clase Servo
Servo miServo;

// Declarar las posiciones del servo
const int posicionInicial = 0; // Posición inicial del servo (A conveniencia)
const int anguloHorario = 180;  // Posición a 180 grados en sentido horario
const int anguloAntihorario = 0; // Posición a 0 grados en sentido antihorario

// ============================== Prototipos de funciones ============================== //
// Prototipos de funciones de configuración
void setup_servo(); 
void setup_pump(); 
void setup_ultrasonic(); 

// Prototipo de funciones de ejecución
float distance_measure();
void water_level_bowl(int distance);
void food_level_bowl(int distance); 
bool water_level_container (float distance);
bool food_level_container(int distance);
void status_checker(); 

// Función de configuración principal 
void setup() {
  // Se inicia el puerto serial
  Serial.begin (9600);
  
  Blynk.begin(auth, ssid, pass);

  // Configuración del servo
  setup_servo(); 
  
  // Configuración de la bomba
  setup_pump(); 

  // Configuración de los sensores ultrasónicos 
  setup_ultrasonic(); 
}

// Función de ejecución principal
void loop() {
  Blynk.run();
  status_checker(); 
}


// Esta función se ejecuta cada vez que el widget de Blynk cambia de estado
BLYNK_WRITE(V0)  // V1 es el Virtual Pin asignado al botón en la aplicación Blynk
{
  pinValue_food = param.asInt();  // Obtiene el valor del pin virtual (0 o 1)
}

// Esta función se ejecuta cada vez que el widget de Blynk cambia de estado
BLYNK_WRITE(V1)  // V1 es el Virtual Pin asignado al botón en la aplicación Blynk
{
  pinValue_water = param.asInt();  // Obtiene el valor del pin virtual (0 o 1)
}

// =============== Definición de funciones de configuración y ejecución =============== //
// Función de configuración del servo motor 
void setup_servo() {
  miServo.attach(pinServo); // Vincular el objeto servo con el pin correspondiente
  
  // Mover el servo a la posición inicial
  miServo.write(posicionInicial);
  delay(1000); // Esperar 1 segundo para asegurarse de que el servo haya llegado a la posición inicial
}

// Función de configuración de la bomba de agua
void setup_pump() {
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, LOW);
}

// Función de configuración del los sensores ultrasónicos 
void setup_ultrasonic() {
  pinMode(trigPin_us1, OUTPUT);
  pinMode(echoPin_us1, INPUT);

  pinMode(trigPin_us2, OUTPUT);
  pinMode(echoPin_us2, INPUT);

  pinMode(trigPin_us3, OUTPUT);
  pinMode(echoPin_us3, INPUT);

  pinMode(trigPin_us4, OUTPUT);
  pinMode(echoPin_us4, INPUT);
}

/**
 * @brief Calcula la distancia en cm en función de la señal recibida de un sensor ultrasónico.
 * 
 * Esta función envía un pulso de 10 microsegundos al pin de disparo del sensor ultrasónico (trigPin),
 * y luego mide el tiempo que tarda en recibir el pulso reflejado en el pin de eco (echoPin). 
 * Utiliza esta duración para calcular y retornar la distancia en centímetros.
 *
 * @param trigPin El pin de disparo del sensor ultrasónico.
 * @param echoPin El pin de recepción del sensor ultrasónico.
 * @return La distancia medida en centímetros.
 */ 
float distance_measure(int trigPin, int echoPin) {
  float duration_us, distance_cm;

  // Enviar un pulso de 10 microsegundos en el pin Trig
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Leer la duración del pulso en el pin Echo
  duration_us = pulseIn(echoPin, HIGH);
  
  // Convertir la duración a distancia en cm
  distance_cm = 0.017 * duration_us;

  return distance_cm; 
}

// Función de monitoreo del contenedor de agua
bool water_level_container (int trigPin, int echoPin){
  float distance = distance_measure(trigPin, echoPin);

  // revisión del nivel de agua del contendor 
  bool empty = false; 

  return empty; 
}

/**
 * @brief Monitorea el nivel de agua en la taza y controla la bomba de agua en consecuencia.
 * 
 * Esta función mide la distancia desde el sensor ultrasónico ubicado en la taza de agua. 
 * Si la distancia medida es mayor o igual a 7 cm, se considera que la taza está vacía y
 * se muestra un mensaje en el serial. Si se recibe una señal de Blynk para llenar la taza,
 * la bomba de agua se activa hasta que la distancia medida sea menor a 4 cm. 
 * Durante este proceso, se envían los valores de nivel de agua a la aplicación Blynk.
 *
 * @param trigPin El pin de disparo del sensor ultrasónico.
 * @param echoPin El pin de recepción del sensor ultrasónico.
 */
void water_level_bowl(int trigPin, int echoPin){
  float distance = distance_measure(trigPin, echoPin);
  float distance_mapped = map(distance,8,0,0,100);
  Blynk.virtualWrite(V3, distance_mapped); // Enviar el valor a Blynk
  if (distance >= 7){
    Serial.println("Taza de agua vacìa. Presione Water para llenar.");
    if(pinValue_water){
      while(distance >= 4){
        digitalWrite(waterPump, HIGH);
        Serial.println("LLENANDO Taza de agua.");
        delay(500);
        distance = distance_measure(trigPin, echoPin);
      }
    }
  }

  if(distance < 4) {
    digitalWrite(waterPump, LOW);
    Serial.println("Taza de agua llena.");
  }
}

// Función de monitoreo del contenedor de comida
bool food_level_container(int trigPin, int echoPin){
  float distance = distance_measure(trigPin, echoPin);

  // revisión del nivel de comida del contendor
  bool empty = false;

  return empty;  
}

/**
 * @brief Monitorea el nivel de comida en la taza y controla el servo motor en consecuencia.
 * 
 * Esta función mide la distancia desde el sensor ultrasónico ubicado en la taza de comida. 
 * Si la distancia medida es mayor o igual a 7 cm, se considera que la taza está vacía y
 * se muestra un mensaje en el serial. Si se recibe una señal de Blynk para llenar la taza,
 * el servo motor se mueve a 180 grados para dispensar comida hasta que la distancia medida 
 * sea menor a 5.5 cm. Durante este proceso, se envían los valores de nivel de comida a la 
 * aplicación Blynk.
 *
 * @param trigPin El pin de disparo del sensor ultrasónico.
 * @param echoPin El pin de recepción del sensor ultrasónico.
 */
void food_level_bowl(int trigPin, int echoPin){
  float distance = distance_measure(trigPin, echoPin);
  Serial.print(distance);
  float distance_mapped = map(distance,8,0,0,100);
  Blynk.virtualWrite(V2, distance_mapped); // Enviar el valor a Blynk
  if (distance >= 7){
    Serial.println("Taza de comida vacìa. Presione food para llenar");
    if(pinValue_food){
      miServo.write(anguloHorario);
      delay(500); // Esperar 0.5 segundos para asegurarse de que el servo haya llegado a la posición
      while(distance>= 5.5){
        Serial.println("Llenando taza de comida");
        delay(500);
        distance = distance_measure(trigPin, echoPin);
        Serial.println("dist2:");
        Serial.println(distance);
      }
    }
  }

  if(distance < 5.5) {
    miServo.write(posicionInicial);
    delay(500); // Esperar 0.5 segundos para asegurarse de que el servo haya llegado a la posición
    Serial.println("Taza de comida llena.");
  }
}

/**
 * @brief Revisa los niveles de los contenedores y tazas de comida y agua, y toma acciones en consecuencia.
 * 
 * Esta función verifica los niveles de los contenedores de comida y agua mediante los sensores 
 * ultrasónicos correspondientes. Luego, llama a las funciones para monitorear los niveles en las tazas
 * de comida y agua. Si los niveles en los contenedores son bajos, se envían mensajes a través del 
 * puerto serial para notificar sobre la condición.
 */
void status_checker() {
  bool msr_sensor_w_container = water_level_container(trigPin_us1, echoPin_us1); 
  bool msr_sensor_f_container = food_level_container(trigPin_us2, echoPin_us2);
  water_level_bowl(trigPin_us3, echoPin_us3);
  food_level_bowl(trigPin_us4, echoPin_us4);

  // Acción a tomar cuando el nivel de comida en el contenedor es bajo
  if (msr_sensor_w_container){
    Serial.println("Nivel de agua en el contenedor es bajo.");
  }

  // Acción a tomar cuando el nivel de agua en el contenedor es bajo
  if (msr_sensor_f_container){
    Serial.println("Nivel de comida en el contenedor es bajo.");
  }
}