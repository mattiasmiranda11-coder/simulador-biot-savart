#ifndef SEGMENTO_HPP
#define SEGMENTO_HPP

#include "../fisica/Vector3D.hpp"

struct Segmento {
    Vector3D centro;  // Coordenada (x, y, z) del punto medio del segmento
    Vector3D dl;      // Vector diferencial de longitud (dirección y magnitud)
};

#endif  // SEGMENTO_HPP