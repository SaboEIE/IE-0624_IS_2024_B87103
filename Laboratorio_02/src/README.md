### Universidad de Costa Rica
#### Escuela de Ingeniería Eléctrica
#### Laboratorio de Microcontroladores - IE0624
#### Estudiante: Alexander Rojas Brenes (B86869)
#### I Ciclo 2024
##### Laboratorio 02: GPIOs, Timers y FSM
---
## Descripción
Este laboratorio corresponde a un programa que permite simular el funcionamiento de un crece peatonal unidireccional con un microcontrolador ATtiny4313. 

## Procedimiento de ejecución
1. Ingresar en el directorio `src` y abrir una terminal en este. 
2. Ejecutar el comando `make` para compilar el código en C según las instrucciones descritas en el archivo Makefile. 
3. Abrir el programa de simulación SimulIDE y abrir con este el archivo *cruce.simu*. 
4. En el simulador, hacer click derecho sobre el Attiny4313 y seleccionar la opción "Load firmware". Aquí se deberá cargar el archivo .hex generado en el paso 2. 
5. Para verificar el funcionamiento del programa, presione el el botón que corresponde a la solicitud de parar el tránsito para cruzar. 