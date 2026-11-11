#include "../../include/geometria/Bobina.hpp"

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Bobina::Bobina(double i, double r, double L, int vueltas, int n)
    : Conductor(i), radio(r), longitud(L), numeroVueltas(vueltas), numeroSegmentos(n) {}

std::vector<Segmento> Bobina::generarSegmentos(int numSegmentos) const {
    std::vector<Segmento> segmentos;
    int N = (numSegmentos > 0) ? numSegmentos : numeroSegmentos;
    if (N <= 0)
        N = 1;

    // Ángulo total a recorrer (vueltas * 360 grados)
    double theta_total = 2.0 * M_PI * numeroVueltas;
    double d_theta = theta_total / N;

    // Cuánto avanza en Z por cada radián que gira
    double paso_z = longitud / theta_total;
    double z_inicial = -longitud / 2.0;

    for (int j = 0; j < N; ++j) {
        Segmento s;
        double theta = (j * d_theta) + (d_theta / 2.0);

        // Geometría de una hélice (resorte) en 3D
        s.centro = {radio * std::cos(theta), radio * std::sin(theta), z_inicial + (paso_z * theta)};

        s.dl = {-radio * std::sin(theta) * d_theta, radio * std::cos(theta) * d_theta,
                paso_z * d_theta};

        segmentos.push_back(s);
    }
    return segmentos;
}