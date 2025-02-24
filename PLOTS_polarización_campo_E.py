import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
plt.style.use('bmh')

# Extracción de datos
data = np.genfromtxt("D:\\Sebastian\\Documents\\VSCode\\Herramientas Computacionales\\Proyecto\\Herr_Computacionales_Proyecto\\datos_simulacion.txt", 
                     delimiter=" ",
                     skip_header=1)

# Cálculo del número de matrices de datos
N_arrays = int(len(data[:,0])/200)

# Reorganizamiento de los datos en N_arrays matrices de tamaño (200,6)
data_n = data.reshape(N_arrays, 200, 6)

# Escoger un conjunto de valores aleatorios para ver los datos en tiempor aleatorios
output_number = 10
times = np.random.randint(1, 
                          N_arrays, 
                          size = output_number)

# Graficación de los tiempos aleatorios
fig1, axis1 = plt.subplots()
axis1.plot(data[0:200,1], data[0:200,2], linewidth = 0.5, label = "Estado inicial")
for i in times:
    axis1.plot(data_n[i,:,1], data_n[i,:,2], linewidth = 0.5,
               label=f"t={data_n[i,0,0]}s")
plt.legend()
plt.savefig("test.png")

# Animación de la onda
fig, axis = plt.subplots()
axis.set_title("Animación - Onda")
axis.set_xlim([min(data_n[0,:,1]), max(data_n[0,:,1])])
axis.set_ylim([-1.2, 1.2])
# Creación de la gráfica que se actualizará en cada fotograma
animated_wave, = axis.plot(data_n[0,:,1], data_n[0,:,2], linewidth = 0.5)

# Creación del arreglo que guardará los datos a gráficar de interés (Campo E.)
E = [data_n[i,:,2] for i in range(N_arrays)]

# Función de animación (en cada fotograma actualiza la gráfica establecida)
def update_data(frame):
    animated_wave.set_data(data_n[i,:,1], E[frame])
    return animated_wave

# FuncAnimation con los parámetros necesarios, se usarán 100 fotogramas a un intervalo de 100 ms
animation = FuncAnimation(fig = fig, 
                          func = update_data, 
                          frames = 100, interval = 100)
animation.save("test_anim.gif")



print(data_n[0,0,2])
print(np.shape(data_n))

