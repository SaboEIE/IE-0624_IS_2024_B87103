import csv
import matplotlib.pyplot as plt

# Cargar datos desde el archivo CSV
timestamps = []
data_column1 = []
data_column2 = []
data_column3 = []

# Se extraen los datos de cada columna y se guardan en listas
with open('data.csv', 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        timestamps.append(float(row[0]))
        data_column1.append(float(row[1]))
        data_column2.append(float(row[2]))
        data_column3.append(float(row[3]))

# Graficar los datos
plt.plot(timestamps, data_column1, label='Op temp')
plt.plot(timestamps, data_column2, label='PID')
plt.plot(timestamps, data_column3, label='Out temp')
plt.xlabel('Tiempo (s)')
plt.ylabel('Valor')
plt.title('Datos en función del tiempo')
plt.legend()
plt.grid(True)
plt.show()
