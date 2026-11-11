#include "../../include/geometria/CableRecto.hpp"

// Constructor: inicializa al padre (Conductor) con la corriente y guarda los datos propios
CableRecto::CableRecto(double i, double L, int n) : Conductor(i), longitud(L), numeroSegmentos(n) {}

std::vector<Segmento> CableRecto::generarSegmentos(int numSegmentos) const {
    std::vector<Segmento> segmentos;

    // Si envían un número inválido, usamos el que guardamos en el constructor
    int N = (numSegmentos > 0) ? numSegmentos : numeroSegmentos;
    if (N <= 0)
        N = 1;

    // Matemáticas de discretización:
    double dl_mag = longitud / N;        // Cuánto mide cada pedacito
    double x_inicial = -longitud / 2.0;  // El cable empieza en la mitad negativa

    for (int j = 0; j < N; ++j) {
        Segmento s;
        // Calculamos el centro exacto de este pedacito en el eje X
        s.centro = {x_inicial + (dl_mag * j) + (dl_mag / 2.0), 0.0, 0.0};
        // El vector dl apunta en la dirección positiva del eje X
        s.dl = {dl_mag, 0.0, 0.0};

        segmentos.push_back(s);
    }

    return segmentos;
}