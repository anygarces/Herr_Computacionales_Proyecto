//Polarización del campo eléctrico en el vacío
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

const double PI = 3.1416;

int main() {
    // Parámetros del problema
    const int NX = 200;         // Número de puntos en el espacio
    const double L = 1.0;       // Longitud del dominio
    const double dx = L / NX;   // Paso espacial
    const double CFL = 0.9;     // Condición de estabilidad
    const double dt = CFL * dx; // Paso temporal, según la condición CFL
    const int nsteps = 1000;    // Número de pasos temporales

    // Parámetros para propagación en el vacío
    double lambda = 1.0;
    double k = 2 * PI / lambda;
    double d = L;  // distancia de propagación

    // Vectores E, H actuales y nuevos
    vector<double> E(NX, 0.0), H(NX, 0.0);
    vector<double> E_nuevo(NX, 0.0), H_nuevo(NX, 0.0);

    // Vectores para almacenar los datos de E y H luego de la simulación
    vector<vector<double>> E_total(nsteps + 1, vector<double>(NX, 0.0));
    vector<vector<double>> H_total(nsteps + 1, vector<double>(NX, 0.0));

    // Condición inicial: pulso gaussiano centrado en L/2 para E
    for (int i = 0; i < NX; i++) {
        double x = i * dx;
        E[i] = exp(-pow(x - L / 2, 2) / 0.01);
        H[i] = 0.0;
    }
    // Guardamos los valores iniciales para t = 0
    for (int i = 0; i < NX; i++) {
        E_total[0][i] = E[i];
        H_total[0][i] = H[i];
    }

    // Lax-Wendroff para las ecuaciones de Maxwell en 1D en el vacío
    // E_t = -H_x   y   H_t = -E_x
    // Se actualizan solo los puntos internos; en los bordes se imponen condiciones fijas (por ejemplo, campos nulos)
    for (int n = 0; n < nsteps; n++) {
        // Actualizamos solo los puntos internos (de 1 a NX-2)
        for (int i = 1; i < NX - 1; i++) {
            int im1 = i - 1;
            int ip1 = i + 1;
            
            E_nuevo[i] = E[i]
                         - (dt / (2 * dx)) * (H[ip1] - H[im1])
                         + (dt * dt / (2 * dx * dx)) * (E[ip1] - 2 * E[i] + E[im1]);

            H_nuevo[i] = H[i]
                         - (dt / (2 * dx)) * (E[ip1] - E[im1])
                         + (dt * dt / (2 * dx * dx)) * (H[ip1] - 2 * H[i] + H[im1]);
        }

        // Imponemos condiciones de contorno fijas en los bordes (por ejemplo, E = 0, H = 0)
        E_nuevo[0] = 0.0;
        E_nuevo[NX - 1] = 0.0;
        H_nuevo[0] = 0.0;
        H_nuevo[NX - 1] = 0.0;

        // Guardamos la solución obtenida en este paso temporal
        for (int i = 0; i < NX; i++) {
            E_total[n + 1][i] = E_nuevo[i];
            H_total[n + 1][i] = H_nuevo[i];
        }

        // Actualizamos los campos para el siguiente paso temporal
        E = E_nuevo;
        H = H_nuevo;
    }

    // Guardar los resultados en un archivo de texto
    ofstream archivo("datos_simulacion.txt");
    if (!archivo) {
        cerr << "Error al abrir el archivo para escribir." << endl;
        return 1;
    }

    // Título del archivo: Tiempo, X, E, Ex, Ey, H
    archivo << "Tiempo" << " " << "X" << " " << "E" << " " << "Ex" << " " << "Ey" << " " << "H" << endl;

    // Se escribe cada instante temporal con cada punto del dominio
    for (int n = 0; n <= nsteps; n++) {
        double t = n * dt;
        for (int i = 0; i < NX; i++) {
            double x = i * dx;
            // Cálculo de las componentes de E en 45° de polarización:
            double Ex = E_total[n][i] / sqrt(2.0);
            double Ey = E_total[n][i] / sqrt(2.0);
            archivo << t << " " << x << " " << E_total[n][i] << " " 
                    << Ex << " " << Ey << " " << H_total[n][i] << endl;
        }
        archivo << "\n";  // Línea vacía para separar los instantes de tiempo
    }

    archivo.close();
    cout << "Datos guardados en 'datos_simulacion.txt'" << endl;

    return 0;
}

