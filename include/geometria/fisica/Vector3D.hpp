#pragma once
#include <cmath>

// Componentes de una posicion [m], un desplazamiento [m] o un campo [T].
struct Vector3D {
    double x = 0.0, y = 0.0, z = 0.0;
    Vector3D operator+(Vector3D b) const { return {x+b.x, y+b.y, z+b.z}; }
    Vector3D operator-(Vector3D b) const { return {x-b.x, y-b.y, z-b.z}; }
    Vector3D operator*(double k) const { return {x*k, y*k, z*k}; }
    double punto(Vector3D b) const { return x*b.x + y*b.y + z*b.z; }
    Vector3D cruz(Vector3D b) const {
        return {y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x};
    }
    double norma() const { return std::hypot(x, y, z); }
    bool finito() const {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
    }
};
