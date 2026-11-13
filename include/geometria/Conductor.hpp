#ifndef CONDUCTOR_HPP
#define CONDUCTOR_HPP

#include <vector>

#include "Segmento.hpp"

class Conductor {
   protected:
    double corriente;  // Corriente en Amperios

   public:
    // Constructor
    Conductor(double i) : corriente(i) {}

    // Destructor virtual (obligatorio en clases abstractas C++)
    virtual ~Conductor() = default;

    // Método que devuelve la corriente
    double getCorriente() const { return corriente; }

    // Función virtual pura: Cada figura geométrica la programará distinto
    virtual std::vector<Segmento> generarSegmentos(int numSegmentos) const = 0;
};

#endif  // CONDUCTOR_HPP