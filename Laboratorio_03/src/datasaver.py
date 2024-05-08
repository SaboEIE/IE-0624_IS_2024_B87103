import serial
import time

serial_port = serial.Serial("/tmp/ttyS1", 9600) # Crear objeto adecuado para la comunicación.

archivo = open("data.csv", 'w') # Crear el archivo csv en modo escritura.
start_time = time.time() # Se establece un tiempo inicial.

while True:
    current_time = time.time() - start_time # Tiempo actual.
    
    data = str(serial_port.readline()) # Variable que almacena los datos obtenidos del serial port.
    clean_data = data[2:][:-5] # Eliminar información irrelevante del raw data.
    
    # Agregar tiempo como primer campo.
    clean_data_with_time = f"{round(current_time, 2)}, {clean_data}"
    
    # Impresión en la terminal para verificar el funcionamiento correcto de la extracción.
    print(clean_data_with_time)

    # Se hace un append del dato actual al documento creado.
    archivo = open("data.csv", "a")
    archivo.write(clean_data_with_time + "\n")

