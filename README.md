### Universidad de Costa Rica
#### Escuela de Ingeniería Eléctrica
#### Laboratorio de Microcontroladores - IE0624
#### Estudiantes: 
- Alexander Rojas Brenes (B86869)
- Kendall Saborio Picado (B87103)
#### I Ciclo 2024
##### Laboratorio 03: Arduino: PID, GPIO, ADC y comunicaciones
---
## Descripción
Este laboratorio corresponde a un programa que permite simular el funcionamiento de 
una incubadora de huevos automática basada en Arduino UNO. 

## Procedimiento de ejecución
1. Ingresar en el directorio `./Laboratorio_03/src` y abrir una terminal en este. 
2. Ejecutar el comando `./virt_port.sh` para habilitar los puertos virtuales que permiten la comunicación entre Arduino y la computadora.
3. Abrir el archivo `./incubadora/incubadora.ino` y generar el binario necesario para la ejecución.
4. Abrir el `incubadora.simu` y cargar el firmware (el binario .hex generado).
5. Activar el interruptor dentro del simulador que habilita la comunicación serial.
6. Ejecutar el comando `python3 datasaver.py` en la terminal para captar y almacenar los datos en un .csv.
7. Ejecutar el comando `python3 graph.py` en la erminal para graficar los resultados obtenidos.