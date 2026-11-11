#ifndef CABLERECTO_HPP
#define CABLERECTO_HPP

#include "Conductor.hpp"

class CableRecto : public Conductor {
   private:
    double longitud;
    int numeroSegmentos;

   public:
    CableRecto(double i, double L, int n);
    ~CableRecto() override = default;

    std::vector<Segmento> generarSegmentos(int numSegmentos) const override;
};

#endif  // CABLERECTO_HPP