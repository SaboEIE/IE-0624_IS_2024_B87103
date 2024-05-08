#!/bin/sh

# Permite habilitarl el puerto virtual para iniciar la extracción de datos del Arduino
socat PTY,link=/tmp/ttyS0,raw,echo=0 PTY,link=/tmp/ttyS1,raw,echo 
