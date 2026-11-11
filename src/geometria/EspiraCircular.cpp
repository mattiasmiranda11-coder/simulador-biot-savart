#include "../../include/geometria/EspiraCircular.hpp"

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

EspiraCircular::EspiraCircular(double i, double r, int n)
    : Conductor(i), radio(r), numeroSegmentos(n) {}

std::vector<Segmento> EspiraCircular::generarSegmentos(int numSegmentos) const {
    std::vector<Segmento> segmentos;
    int N = (numSegmentos > 0) ? numSegmentos : numeroSegmentos;
    if (N <= 0)
        N = 1;

    // Discretización angular: dividimos 360 grados (2*PI radianes) en N partes
    double d_theta = (2.0 * M_PI) / N;

    for (int j = 0; j < N; ++j) {
        Segmento s;
        // Ángulo en el centro exacto de este segmento
        double theta = (j * d_theta) + (d_theta / 2.0);

        // Coordenadas espaciales usando trigonometría (plano XY)
        s.centro = {radio * std::cos(theta), radio * std::sin(theta), 0.0};

        // El vector dl es tangente a la circunferencia (derivada de la posición)
        s.dl = {-radio * std::sin(theta) * d_theta, radio * std::cos(theta) * d_theta, 0.0};

        segmentos.push_back(s);
    }
    return segmentos;
}