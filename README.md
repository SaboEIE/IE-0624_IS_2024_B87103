### Universidad de Costa Rica
#### Escuela de Ingeniería Eléctrica
#### Laboratorio de Microcontroladores - IE0624
#### Estudiantes: 
- Alexander Rojas Brenes (B86869)
- Kendall Saborio Picado (B87103)
#### I Ciclo 2024
##### Laboratorio 05: Arduino Nano 33 BLE: GPIO y TinyML
---
## Descripción
Este laboratorio corresponde al entrenamiento de un modelo encargado de la identificación de mascarillas utilizadando el kit de Tiny Machine Learning de Arduino. 

## Procedimiento de ejecución
1. En primer lugar, es necesario instalar la última versión del Arduino IDE (2.0.1) y el soporte Arduino Mbed OS Nano boards(v3.4.1). 
2. Instalar [Arduino CLI](https://arduino.github.io/arduino-cli/0.28/). 
3. Instalar biblioteca Arduino `TensorFlowLite` y `Arduino:LSM9DS1`. 
4. Crearse una cuenta en [Edge Impulse](https://edgeimpulse.com/).
5. Instalar el [edge-impulse-cli](https://docs.edgeimpulse.com/docs/edge-impulse-cli/cli-installation).
6. Conectar el microcontrolador a la PC.
7. Complilar y cargar el código el códifo contenido en el archivo `mask_identification_model.ino` utilizando el Arduino IDE.
8. Apuntar la cámara del kit al objeto que quiera indentificarse en función de las tres clases programadas y observar los cambios del LED RGB. 