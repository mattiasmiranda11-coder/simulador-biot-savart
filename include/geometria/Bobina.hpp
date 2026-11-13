#ifndef BOBINA_HPP
#define BOBINA_HPP

#include "Conductor.hpp"

class Bobina : public Conductor {
   private:
    double radio;
    double longitud;
    int numeroVueltas;
    int numeroSegmentos;

   public:
    Bobina(double i, double r, double L, int vueltas, int n);
    ~Bobina() override = default;

    std::vector<Segmento> generarSegmentos(int numSegmentos) const override;
};

#endif  // BOBINA_HPP