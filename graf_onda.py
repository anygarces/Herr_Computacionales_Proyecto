# animate_wave_1d.py
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

def read_wave_data(filename):
    """
    Lee el archivo 'wave_data.txt' que contiene:
      FRAME n
      u0 u1 u2 ... u(NX-1)
    y devuelve una lista de arrays (frames), 
    donde cada array es el estado de la onda en un paso de tiempo.
    """
    frames = []
    with open(filename, 'r') as f:
        lines = f.read().splitlines()

    temp_data = []
    for line in lines:
        line = line.strip()
        if line.startswith("FRAME"):
            # Si temp_data tiene algo, lo convertimos en array y lo guardamos
            if temp_data:
                frames.append(np.array(temp_data, dtype=float))
                temp_data = []
        elif line == '':
            # Línea en blanco, se ignora
            continue
        else:
            # Se asume que la línea contiene valores de la onda
            values = line.split()
            for v in values:
                temp_data.append(float(v))

    # Agregar el último frame si quedó pendiente
    if temp_data:
        frames.append(np.array(temp_data, dtype=float))

    return frames


def main():
    # Parámetros del dominio espacial (deben coincidir con el C++)
    x_min, x_max = -100.0, 100.0
    NX = 201
    x = np.linspace(x_min, x_max, NX)

    # Leer los datos del archivo
    filename = "wave_data.txt"
    frames = read_wave_data(filename)
    nframes = len(frames)
    print(f"Se han leído {nframes} frames desde '{filename}'.")

    # Configurar figura y ejes
    fig, ax = plt.subplots()
    # Graficar la primera solución
    line, = ax.plot(x, frames[0], 'b-', lw=2)
    ax.set_xlim(x_min, x_max)
    # Ajustar los límites verticales según la amplitud esperada
    ax.set_ylim(-1.2, 1.2)
    ax.set_xlabel("x")
    ax.set_ylabel("u(x,t)")
    ax.set_title("Ecuación de Onda 1D - Lax–Wendroff")

    # Función de actualización para cada frame
    def animate(i):
        line.set_ydata(frames[i])
        ax.set_title(f"Ecuación de Onda 1D - Frame {i}")
        return line,

    # Crear la animación
    anim = animation.FuncAnimation(fig, 
                                   animate, 
                                   frames=350, 
                                   interval=50, 
                                   blit=True)

    # Guardar la animación como GIF (requiere 'pillow': pip install pillow)
    anim.save("wave_simulation.gif", writer='pillow', fps=20)
    print("GIF guardado como 'wave_simulation.gif'.")

    # Mostrar la animación en pantalla
    #plt.show()


if __name__ == "__main__":
    main()