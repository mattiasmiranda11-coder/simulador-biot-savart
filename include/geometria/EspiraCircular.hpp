#ifndef ESPIRACIRCULAR_HPP
#define ESPIRACIRCULAR_HPP

#include "Conductor.hpp"

class EspiraCircular : public Conductor {
   private:
    double radio;
    int numeroSegmentos;

   public:
    EspiraCircular(double i, double r, int n);
    ~EspiraCircular() override = default;

    std::vector<Segmento> generarSegmentos(int numSegmentos) const override;
};

#endif  // ESPIRACIRCULAR_HPP