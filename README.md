### Universidad de Costa Rica
#### Escuela de Ingeniería Eléctrica
#### Laboratorio de Microcontroladores - IE0624
#### Estudiantes: 
- Alexander Rojas Brenes (B86869)
- Kendall Saborio Picado (B87103)
#### I Ciclo 2024
##### Laboratorio 04: STM32: GPIO, ADC, timer, comunicaciones, Iot
---
## Descripción
Este laboratorio corresponde a la construcción de un sistema que permite monitorear los cambios en la pendiente de un terreno y los despliega en la plataforma Thingsboard. 

## Procedimiento de ejecución
1. En primer lugar, se instala el toolchain de ARM para poder compilar la biblioteca LibOpenCM3 y sus ejemplos, para esto se deben seguir los pasos detallados en el siguiente link: [ARM Toolchain](https://launchpad.net/gcc-arm-embedded).
2. Se descarga e instala la libería siguiendo los pasos del siguiente link: [LibOpenCM3 Wiki](https://github.com/libopencm3/libopencm3/wiki).
3. Descargar los ejemplos propuestos por la librería que se encuentran en el siguiente link: [LibOpenCM3 Examples](https://github.com/libopencm3/libopencm3-examples). 
4. Ejecutar el siguiente comando: `cd examples` y copiar el contenido construido e instalado en el paso 2 en el directorio "libopencm3".
5. Ejecutar `make` en el presente directorio.
6. Ejecutar el siguientes comandos: 
    - `cd examples/examples/stm32/f4/stm32f429i-discovery/`
    - `mkdir slope_monitoring`
7. Copiar el contenido de `./Laboratorio_04/src` en este directorio, lo cual permite que los archivos del laboratorio se encuentran en un ambiente apropiado para su construcción y ejecución. 
8. Conectar el microcontrolador a la PC. 
9. Para construir y "flashear" el código, se ejecuta el comando: `make flash V=1`. 
10. Para iniciar con el envío de datos a Thingsboard, se debe presionar el botón en el microcontrolador que permite iniciar con la transmisión (botón azul) y se ejecuta el siguiente comando: `python3 iot.py`. 