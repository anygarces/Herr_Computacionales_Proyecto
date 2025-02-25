//Polarización del campo eléctrico en el vacío
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

const double PI = 3.1416;

// Parámetros del problema
const int NX = 200;         // Número de puntos en el espacio
const double L = 1.0;       // Longitud del dominio
const double dx = L / NX;   // Paso espacial (h)
const double CFL = 0.9;     // Condición de estabilidad
const double dt = CFL * dx; // Paso temporal, según la condición CFL
const int nsteps = 1000;    // Número de pasos temporales

// Parámetros para propagación en el vacío
double lambda = 1.0;
double c = 1.0;
double k = 2 * PI / lambda;
double d = L;  // distancia de propagación

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

    // Tomando un campo eléctrico únicamente en el eje x (polarización lineal)

    // dE/dt = v
    // dv/dt = c^2 dw/dx
    // dw/dt = dv/dx 

    // Estas son 3 ecuaciones de primer orden para Lax-Wendroff, con
     
    // v = dE/dt   y   v(x,0) = -c dE/dx (x,0)
    // w = dE/dx   y   w(x,0) =    dE/dx (x,0) 

    // Con condiciones iniciales 

    // E(x,0) = f(x)
    // dE/dt (x,0) = -c dE/dx (x,0)


    // Vectores de E,v y w

    vector<double> E(NX, 0.0), v(NX, 0.0), w(NX, 0.0);
    vector<double> E_nuevo(NX, 0.0), v_nuevo(NX, 0.0), w_nuevo(NX, 0.0);

    // Vectores para almacenar los datos de E luego de la simulación
    vector<vector<double>> E_total(nsteps + 1, vector<double>(NX, 0.0));
    vector<vector<double>> v_total(nsteps + 1, vector<double>(NX, 0.0));
    vector<vector<double>> w_total(nsteps + 1, vector<double>(NX, 0.0));

    // Escritura valores iniciales

    for (int i = 0; i < NX; i++) {
        double x = i * dx;
        E[i] = E_0(x);
        v[i] = v_0(x);
        w[i] = w_0(x);
    }
    // Guardamos los valores iniciales para t = 0
    for (int i = 0; i < NX; i++) {
        E_total[0][i] = E[i];
        v_total[0][i] = v[i];
        w_total[0][i] = w[i];
    }

    // Lax-Wendroff para las ecuaciones presentadas en E, v y w
    // Se actualizan solo los puntos internos; en los bordes se imponen condiciones fijas (por ejemplo, campos nulos)
    for (int n = 0; n < nsteps; n++) {
        // Actualizamos solo los puntos internos (de 1 a NX-2)
        for (int i = 1; i < NX - 1; i++) {
            
            E_nuevo[i] = E[i] + dt*v[i] + (pow(dt*c,2)/(2*pow(dx,2)))*(E[i+1] - 2*E[i] + E[i-1]);
            v_nuevo[i] = v[i] + ((dt*pow(c,2))/(2*dx))*(w[i+1]-w[i-1]) + (pow(dt*c,2)/(2*pow(dx,2)))*(v[i+1] - 2*v[i] + v[i-1]);
            w_nuevo[i] = w[i] + (dt/(2*dx))*(v[i+1]-v[i-1]) + (pow(dt*c,2)/(2*pow(dx,2)))*(w[i+1] - 2*w[i] + w[i-1]);
        
        }

        // Imponemos condiciones de contorno fijas en los bordes (por ejemplo, E = 0, H = 0)
        E_nuevo[0] = E_nuevo[NX - 1];
        v_nuevo[0] = v_nuevo[NX - 1];
        w_nuevo[0] = w_nuevo[NX - 1];  

        // Guardamos la solución obtenida en este paso temporal
        for (int i = 0; i < NX; i++) {
            E_total[n + 1][i] = E_nuevo[i];
        }

        // Actualizamos los campos para el siguiente paso temporal
        E = E_nuevo;
    }

    // Guardar los resultados en un archivo de texto
    ofstream archivo("datos_simulacion2.txt");
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
    cout << "Datos guardados en 'datos_simulacion2.txt'" << endl;

    return 0;
}
