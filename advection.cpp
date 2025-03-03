//Polarización del campo eléctrico en el vacío
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

const double PI = 3.1416;

// Parámetros del problema

// Parámetros para propagación en el vacío
double lambda = 1.0;
double c = 3.0e8;
double k = 2 * PI / lambda;

const int NX = 200;         // Número de puntos en el espacio
const double L = 1.0;       // Longitud del dominio
const double dx = L / (NX-1);   // Paso espacial (h)
const double CFL = 0.5;     // Condición de estabilidad
const double dt = CFL * dx/c; // Paso temporal, según la condición CFL
const int nsteps = 1000;    // Número de pasos temporales

// Condición inicial: pulso gaussiano centrado en L/2 para E

double E_0 (double x) {
    return exp(-pow(x - L / 2, 2) / 0.01);
 }

// v(x,0) = -c dE/dx (x,0)

double v_0 (double x) {
    return -c*(-100*(2*x - L))*E_0(x); 
}

// w(x,0) =    dE/dx (x,0)
    
double w_0 (double x) {
    return -100*(2*x - L)*E_0(x);
}

int main() {

   // Resolver dE = -c dE
   //          dt      dx

    vector<double> E(NX, 0.0);
    vector<double> E_nuevo(NX, 0.0);

    // Vectores para almacenar los datos de E luego de la simulación
    vector<vector<double>> E_total(nsteps + 1, vector<double>(NX, 0.0));

    // Escritura valores iniciales

    for (int i = 0; i < NX; i++) {
        double x = i * dx;
        E[i] = E_0(x);
    }
    // Guardamos los valores iniciales para t = 0
    for (int i = 0; i < NX; i++) {
        E_total[0][i] = E[i];
    }

    // Lax-Wendroff para las ecuaciones presentadas en E
    // Se actualizan solo los puntos internos; en los bordes se imponen condiciones fijas (por ejemplo, campos nulos)
    
    for (int n = 0; n < nsteps; n++) {
        // Imponemos condiciones de contorno periodicas
        E_nuevo[0] = E_nuevo[NX - 2];
        E_nuevo[NX - 1] = E_nuevo[1];
        for (int i = 1; i < NX - 1 ; i++) {
            
            E_nuevo[i] = E[i] 
                        -(c*dt/(2*dx))*(E[i+1]-E[i-1]) + (pow(c*dt,2)/(2*pow(dx,2)))*(E[i+1] - 2*E[i] + E[i-1]);     
        
        }

        // Guardamos la solución obtenida en este paso temporal
        for (int i = 0; i < NX; i++) {
            E_total[n][i] = E_nuevo[i];
        }

        // Actualizamos los campos para el siguiente paso temporal
        E = E_nuevo;
    }

    // Guardar los resultados en un archivo de texto
    ofstream archivo("datos_simulacionA.txt");
    if (!archivo) {
        cerr << "Error al abrir el archivo para escribir." << endl;
        return 1;
    }

    // Título del archivo: Tiempo, X, E, Ex, Ey, H
    archivo << "t" << " " << "X" << " " << "E" << endl;

    // Se escribe cada instante temporal con cada punto del dominio
    for (int n = 0; n <= nsteps; n++) {
        double t = n * dt;
        for (int i = 0; i < NX; i++) {
            double x = i * dx;
            archivo << t << " " << x << " " << E_total[n][i] << endl;
        }
        //archivo << "\n";  // Línea vacía para separar los instantes de tiempo
    }

    archivo.close();
    cout << "Datos guardados en 'datos_simulacionA.txt'" << endl;

    return 0;
}