import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
plt.style.use('bmh')

# Extracción de datos
data = np.genfromtxt("D:\Sebastian\Documents\VSCode\Herramientas Computacionales\Proyecto\Herr_Computacionales_Proyecto\datos_simulacion1.txt", 
                     delimiter=" ",
                     skip_header=1)

# Cálculo del número de matrices de datos
N_arrays = int(len(data[:,0])/200)

# Reorganizamiento de los datos en N_arrays matrices de tamaño (200,6)
data_n = data.reshape(N_arrays, 200, 6)

# Animación de la onda
fig, ax = plt.subplots()
ax.set_title("Animación - Onda")
ax.set_xlim([min(data_n[0,:,1]), max(data_n[0,:,1])])
ax.set_ylim([-0.05, 1.05])
# Creación de la gráfica que se actualizará en cada fotograma
animated_wave, = ax.plot(data_n[0,:,1], data_n[0,:,2], linewidth = 0.5)

# Creación del arreglo que guardará los datos a gráficar de interés (Campo E.)
E = [data_n[i,:,2] for i in range(N_arrays)]

# Función de animación (en cada fotograma actualiza la gráfica establecida)
def update_data(frame):
    animated_wave.set_data(data_n[frame,:,1], E[frame])
    return animated_wave

# FuncAnimation con los parámetros necesarios, se usarán 100 fotogramas a un intervalo de 100 ms
animation = FuncAnimation(fig = fig, 
                          func = update_data, 
                          frames = 200, interval = 100)
animation.save("test_anim1.gif")





print(data_n[0,0,2])
print(np.shape(data_n))



print(data_n[0,0,2])
print(np.shape(data_n))

