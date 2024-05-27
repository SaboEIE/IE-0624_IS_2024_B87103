import serial  # Módulo para comunicación serial
import time    # Módulo para manejar tiempos
import json    # Módulo para manipular datos JSON
import paho.mqtt.client as mqtt  # Módulo para MQTT

def on_connect(client, userdata, flags, rc):
    """
    Callback para cuando el cliente MQTT se conecta al broker MQTT.

    Esta función se llama automáticamente cuando el cliente MQTT se conecta 
    al broker MQTT. Verifica si la conexión fue exitosa o no e imprime un mensaje correspondiente.

    Args:
        client: El cliente MQTT que se conectó al broker.
        userdata: Los datos del usuario (no se utilizan en este caso).
        flags: Banderas de conexión (no se utilizan en este caso).
        rc: Código de resultado de conexión. 0 indica conexión exitosa.

    Returns:
        None
    """
    if rc == 0:
        client.connected = True  # Marcar que la conexión fue exitosa
        print("Conexión exitosa.")
    else: 
        print("Conexión fallida: ", rc)
        client.loop_stop()  # Detener el bucle del cliente MQTT si la conexión falla

def conn_check(client):
    """
    Verifica continuamente si el cliente MQTT está conectado al broker.

    Esta función ejecuta un bucle continuo para verificar si el cliente MQTT 
    está conectado al broker MQTT. Mientras el cliente no esté conectado, 
    llama al método `loop` del cliente MQTT para procesar los mensajes 
    entrantes y salientes, y espera 2 segundos antes de realizar la siguiente verificación.

    Args:
        client: El cliente MQTT que se está verificando.

    Returns:
        None
    """
    while client.connected != True:  # Mientras el cliente MQTT no esté conectado
        client.loop()  # Procesar mensajes entrantes y salientes
        time.sleep(2)  # Esperar 2 segundos antes de realizar la siguiente verificación