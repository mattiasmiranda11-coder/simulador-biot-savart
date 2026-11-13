#pragma once
#include <optional>
#include "../geometria/Conductor.hpp"

class BiotSavartSolver {
public:
    // Metros. Es un limite numerico, no el radio real del conductor.
    explicit BiotSavartSolver(double exclusion = 0.001);
    // nullopt = punto excluido; excepcion = entrada invalida.
    std::optional<Vector3D> calcularCampo(const Conductor& conductor,
                                         Vector3D punto, int numeroSegmentos) const;
    std::optional<Vector3D> calcularCampo(const std::vector<Segmento>& segmentos,
                                         double corriente, Vector3D punto) const;
private:
    double exclusion_;
};
