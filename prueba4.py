import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

def leer_datos(filename):
    """
    Lee el archivo 'datos_simulacion.txt' y organiza los datos en bloques.
    Cada bloque corresponde a un instante temporal y tiene las columnas:
    Tiempo, X, E, Ex, Ey, H.
    """
    with open(filename, 'r') as f:
        lines = f.readlines()

    bloques = []
    bloque_actual = []
    for line in lines:
        if line.strip() == "":
            if bloque_actual:
                bloques.append(bloque_actual)
                bloque_actual = []
        elif "Tiempo" in line:  # se salta la cabecera
            continue
        else:
            bloque_actual.append(line.strip())
    if bloque_actual:
        bloques.append(bloque_actual)

    datos = []
    for bloque in bloques:
        xs = []
        Es = []
        Exs = []
        Eys = []
        Hs = []
        tiempos = []
        for fila in bloque:
            partes = fila.split()
            # Se espera: Tiempo, X, E, Ex, Ey, H
            tiempo = float(partes[0])
            x = float(partes[1])
            E_val = float(partes[2])
            Ex_val = float(partes[3])
            Ey_val = float(partes[4])
            H_val = float(partes[5])
            tiempos.append(tiempo)
            xs.append(x)
            Es.append(E_val)
            Exs.append(Ex_val)
            Eys.append(Ey_val)
            Hs.append(H_val)
        # Se asume que el tiempo es el mismo para todas las filas del bloque
        datos.append({
            'time': tiempos[0],
            'x': np.array(xs),
            'E': np.array(Es),
            'Ex': np.array(Exs),
            'Ey': np.array(Eys),
            'H': np.array(Hs)
        })
    return datos

# Lectura de los datos
datos = leer_datos("datos_simulacion.txt")
num_frames = len(datos)

# Configuración de la figura y ejes
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), sharex=True)
# Panel para el campo eléctrico: E total, Ex y Ey
line_E, = ax1.plot([], [], 'k-', lw=2, label='E total')
line_Ex, = ax1.plot([], [], 'b--', lw=2, label='Ex')
line_Ey, = ax1.plot([], [], 'r--', lw=2, label='Ey')
ax1.set_ylabel("Campo Eléctrico")
ax1.legend(loc='upper right')
ax1.set_title("Evolución de los Campos Electromagnéticos")

# Panel para el campo magnético H
line_H, = ax2.plot([], [], 'g-', lw=2, label='H')
ax2.set_ylabel("Campo Magnético")
ax2.set_xlabel("Posición x")
ax2.legend(loc='upper right')

# Ajuste de los límites de los ejes en función de los datos del primer bloque
x_inicial = datos[0]['x']
ax1.set_xlim(np.min(x_inicial), np.max(x_inicial))
ax2.set_xlim(np.min(x_inicial), np.max(x_inicial))

# Establecer límites globales para los ejes y
E_total_all = np.concatenate([frame['E'] for frame in datos])
Ex_all = np.concatenate([frame['Ex'] for frame in datos])
Ey_all = np.concatenate([frame['Ey'] for frame in datos])
H_all = np.concatenate([frame['H'] for frame in datos])
ax1.set_ylim(min(np.min(E_total_all), np.min(Ex_all), np.min(Ey_all)),
             max(np.max(E_total_all), np.max(Ex_all), np.max(Ey_all)))
ax2.set_ylim(np.min(H_all), np.max(H_all))

# Texto para mostrar el tiempo actual en el panel superior
time_text = ax1.text(0.05, 0.9, '', transform=ax1.transAxes, fontsize=12)

def init():
    """Función de inicialización para la animación."""
    line_E.set_data([], [])
    line_Ex.set_data([], [])
    line_Ey.set_data([], [])
    line_H.set_data([], [])
    time_text.set_text('')
    return line_E, line_Ex, line_Ey, line_H, time_text

def animate(i):
    """Actualiza los datos para el frame i."""
    frame = datos[i]
    x = frame['x']
    line_E.set_data(x, frame['E'])
    line_Ex.set_data(x, frame['Ex'])
    line_Ey.set_data(x, frame['Ey'])
    line_H.set_data(x, frame['H'])
    time_text.set_text(f"Tiempo = {frame['time']:.3f}")
    return line_E, line_Ex, line_Ey, line_H, time_text

# Crear la animación
ani = animation.FuncAnimation(fig, animate, frames=num_frames,
                              init_func=init, interval=50, blit=True)

plt.tight_layout()
plt.show()
