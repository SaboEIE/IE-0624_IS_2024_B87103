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
        
def setup():
    """
    Configura la conexión serial, el cliente MQTT y otros parámetros necesarios.

    Esta función configura la comunicación serial con el dispositivo,
    crea un cliente MQTT, establece los parámetros de conexión al broker MQTT,
    y define un diccionario para almacenar los datos a enviar.

    Returns:
        tuple: Una tupla que contiene los siguientes elementos en orden:
               - datos: El objeto serial para la comunicación con el dispositivo.
               - diccionario: Un diccionario para almacenar los datos a enviar.
               - client: El cliente MQTT configurado y conectado al broker.
               - topic: El tema MQTT al que se enviarán los datos.
    """
    datos = serial.Serial("/dev/ttyACM0", 115200, timeout=1)  # Configurar la comunicación serial con el dispositivo
    client = mqtt.Client()  # Crear un cliente MQTT
    client.connected = False  # Inicializar la bandera de conexión del cliente MQTT
    client.on_connect = on_connect  # Establecer la función de devolución de llamada para la conexión al broker MQTT

    broker = "iot.eie.ucr.ac.cr"  # Dirección del broker MQTT
    port = 1883  # Puerto del broker MQTT
    topic = "v1/devices/me/telemetry"  # Tema MQTT al que se enviarán los datos
    device = "q2IvRYzJf7zHTMSR0PKU"  # Identificador del dispositivo MQTT
    client.username_pw_set(device)  # Establecer el identificador del dispositivo como usuario para la conexión al broker MQTT
    client.connect(broker, port)  # Conectar el cliente MQTT al broker MQTT

    diccionario = dict()  # Crear un diccionario para almacenar los datos a enviar

    return datos, diccionario, client, topic  # Retornar los objetos y parámetros configurados
