# animate_wave_1d.py
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
plt.style.use('bmh')

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
    
    CFE = [0.1, 0.3, 0.5, 0.7]
    frames_cfe = []

    filename1 = f"wave_data_0.100000.txt"
    frames1 = read_wave_data(filename1)
    frames_cfe.append(frames1)
    nframes1 = len(frames1)
    print(f"Se han leído {nframes1} frames desde '{filename1}'.")

    filename2 = f"wave_data_0.300000.txt"
    frames2 = read_wave_data(filename2)
    frames_cfe.append(frames2)
    nframes2 = len(frames2)
    print(f"Se han leído {nframes2} frames desde '{filename2}'.")

    filename3 = f"wave_data_0.500000.txt"
    frames3 = read_wave_data(filename3)
    frames_cfe.append(frames3)
    nframes3 = len(frames3)
    print(f"Se han leído {nframes3} frames desde '{filename3}'.")

    filename4 = f"wave_data_0.700000.txt"
    frames4 = read_wave_data(filename4)
    frames_cfe.append(frames4)
    nframes4 = len(frames4)
    print(f"Se han leído {nframes4} frames desde '{filename4}'.")

    # Configurar figura y ejes
    fig, ax = plt.subplots()
    
    # Crear lista con los gráficos de cada valor de CFE

    # Elegir entre crear gráficas para todos los valores de CFE o sólo para uno en específico

    # Para graficarlos todos

    #lines = []
    #for j, cfe in enumerate(CFE):
    #    line, = ax.plot(x, frames_cfe[j][0], lw=2, label=f"cfe = {cfe}")
    #    lines.append(line)
    #    ax.legend()

    # Para analizar un valor en específico

    k = 2

    line, = ax.plot(x, frames_cfe[k][0], lw=2, label=f"cfe = {CFE[2]}")
    ax.legend(loc = 'best')
    ax.set_xlim(x_min, x_max)
    # Ajustar los límites verticales según la amplitud esperada
    ax.set_ylim(-1.2, 1.2)
    ax.set_xlabel("x")
    ax.set_ylabel("u(x,t)")
    ax.set_title("Ecuación de Onda 1D - Lax–Wendroff")

    #ax.plot(x, frames[100])

    # Función de actualización para cada frame
    def animate(i):
        #for j, line in pairs:
        #    line.set_ydata(frames_cfe[j][i])
        #    ax.set_title(f"Ecuación de Onda 1D - Frame {i}")
        line.set_ydata(frames_cfe[k][i])
        ax.set_title(f"Ecuación de Onda 1D - Frame {i}")
        return [line]

    # Crear la animación
    anim = animation.FuncAnimation(fig, 
                                   animate, 
                                   frames=nframes1, 
                                   interval=50, 
                                   blit=True)

    # Guardar la animación como GIF (requiere 'pillow': pip install pillow)
    anim.save("wave_simulation.gif", writer='pillow', fps=20)
    print("GIF guardado como 'wave_simulation.gif'.")

    # Mostrar la animación en pantalla
    #plt.show()


if __name__ == "__main__":
    main()
