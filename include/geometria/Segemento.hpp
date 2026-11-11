#ifndef SEGMENTO_HPP
#define SEGMENTO_HPP

// Estructura simple para representar un vector en 3D
struct Vector3D {
    double x, y, z;
};

// Estructura que representa un diferencial de corriente (dl) en el espacio
struct Segmento {
    Vector3D centro;  // Punto medio del segmento (x, y, z)
    Vector3D dl;      // Vector de longitud y dirección del segmento
};

#endif  // SEGMENTO_HPP