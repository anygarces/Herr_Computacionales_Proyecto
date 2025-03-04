#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip> 
#include <string>
using namespace std;

// Función gaussiana para la condición inicial
double gaussian(double x, double x0, double sigma) {
    return exp(-0.5 * ( (x - x0)*(x - x0) ) / (sigma*sigma));
}

// Derivada espacial de la gaussiana (para la condición inicial de velocidad)
double gaussian_deriv(double x, double x0, double sigma) {
    // f'(x) = - (x - x0)/sigma^2 * gauss(x)
    return - (x - x0)/(sigma*sigma) * gaussian(x, x0, sigma);
}

int main() {

    // Valores de dt que se buscan contrastat
    vector<double> CFL = {0.1, 0.3, 0.5, 0.7};

    // Parámetros del dominio espacial
    const int    NX    = 201;   // número de puntos en x
    const double x_min = -100.0;
    const double x_max =  100.0;
    const double dx    = (x_max - x_min) / (NX - 1);

    // Parámetros de la simulación temporal
    const double c     = 1.0;   // velocidad de propagación
    const int    NT    = 700;   // número de pasos de tiempo

    // Parámetros para la condición inicial (pulso gaussiano)
    const double x0    = -50.0; // centro inicial del pulso
    const double sigma = 10.0;  // anchura del pulso

    // Condiciones de frontera (fixed == 1 para fronteras fijas, fixed == 0 para fronteras periodicas)

    int fijas;

    cout << "Fronteras fijas (1) o periodicas (0)" << endl;
    cin >> fijas;

    for (int i=0; i<5; i++){

        const double cfl = CFL[i];   // paso de tiempo

        // Cálculo de lambda para el esquema
        double dt = cfl * dx/c;
        double lambda = (c * dt) / dx;
        // Comprobación básica de estabilidad (no estricta, pero orientativa)
        if (lambda >= 1.0) {
            cerr << "ADVERTENCIA: lambda = c*dt/dx >= 1, puede ser inestable.\n";
        }

        // Vectores para almacenar la onda en tres instantes: n-1, n, n+1
        vector<double> u_prev(NX, 0.0);
        vector<double> u_curr(NX, 0.0);
        vector<double> u_next(NX, 0.0);

        // Vector de coordenadas x para referencia
        vector<double> xcoord(NX);
        for(int j = 0; j < NX; j++) {
            xcoord[j] = x_min + j * dx;
        }

        // Condiciones iniciales
        // u^n=0 (n=0)
        // 1) u_curr: gaussiana en t=0
        for(int j = 0; j < NX; j++) {
            u_curr[j] = gaussian(xcoord[j], x0, sigma);
        }
        // 2) Para avanzar al primer paso (n=1), usamos la derivada temporal en t=0
        //    u^1_j = u^0_j + dt * (du/dt)^0_j
        for(int j = 0; j < NX; j++) {
            double du_dt = -c * gaussian_deriv(xcoord[j], x0, sigma); // pulso viajando a la derecha
            u_next[j]    = u_curr[j] + dt * du_dt;
        }
        // Pasar a n=1
        u_prev = u_curr;
        u_curr = u_next;

        // Abrir archivo de salida
        string file_name = "wave_data_" + to_string(lambda) + ".txt";
        ofstream outfile (file_name);
        if(!outfile.is_open()) {
            cerr << "No se pudo abrir" << file_name << "para escritura.\n";
            return -1;
        }

        // Escribir FRAME 0 (el estado inicial real)
        outfile << "FRAME 0\n";
        for(int j = 0; j < NX; j++) {
            outfile << fixed << setprecision(6) << u_prev[j] << " ";
        }
        outfile << "\n\n";

        // Escribir FRAME 1
        outfile << "FRAME 1\n";
        for(int j = 0; j < NX; j++) {
            outfile << fixed << setprecision(6) << u_curr[j] << " ";
        }
        outfile << "\n\n";

        // Bucle principal de tiempo: desde n=1 hasta NT-1
        for(int n = 2; n <= NT; n++) {
            // Actualizar u_next con el esquema Lax-Wendroff (1D)
            // Para j=2..NX-3, pues se necesitan vecinos hasta ±2
            for(int j = 2; j < NX - 2; j++) {
                // Términos del esquema:
                double term2 = ( u_curr[j+1] - 2*u_curr[j] + u_curr[j-1] );
                double term4 = ( u_curr[j+2] - 4*u_curr[j+1] + 6*u_curr[j]
                               - 4*u_curr[j-1] + u_curr[j-2] );

                u_next[j] = 2.0*u_curr[j] - u_prev[j]
                            + lambda*lambda * term2
                            + 0.5 * lambda*lambda*lambda*lambda * term4;
            }

            // Condiciones de contorno (Dirichlet = 0)
            // En las zonas donde no se aplica el esquema (j < 2 o j >= NX-2), fijamos 0
            
            if (fijas == 1){
                u_next[0]      = 0;
                u_next[NX-1]   = 0;
                u_next[1]      = 0;
                u_next[NX-2]   = 0;
            } else if (fijas == 0){
                u_next[0]      = u_next[NX-4];
                u_next[NX-1]   = u_next[3];
                u_next[1]      = u_next[NX-3];
                u_next[NX-2]   = u_next[2];
            }
            // Guardar el frame en el archivo
            outfile << "FRAME " << n << "\n";
            for(int j = 0; j < NX; j++) {
                outfile << fixed << setprecision(6) << u_next[j] << " ";
            }
            outfile << "\n\n";

            // Avanzar en el tiempo
            u_prev = u_curr;
            u_curr = u_next;
        };

        outfile.close();
        cout << "Simulacion finalizada. Datos guardados en" << file_name << "wave_data.txt'.\n";
    
    }

    return 0;
}
