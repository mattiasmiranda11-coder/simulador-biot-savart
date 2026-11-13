#pragma once
#include "fisica/BiotSavartSolver.hpp"

// Compatibilidad con el benchmark de Benjamin y las operaciones del prototipo.
// La implementacion fisica es unica: BiotSavartSolver.
namespace magnesim {
using Vec3 = Vector3D;
struct Segmento { Vec3 inicio, fin; };
inline double producto_punto(Vec3 a, Vec3 b) { return a.punto(b); }
inline Vec3 producto_cruz(Vec3 a, Vec3 b) { return a.cruz(b); }
inline std::optional<Vec3> calcular_campo(const std::vector<Segmento>& tramos,
                                         double corriente, Vec3 punto,
                                         double exclusion = 0.001) {
    std::vector<::Segmento> segmentos;
    segmentos.reserve(tramos.size());
    for (const auto& s : tramos)
        segmentos.push_back({s.inicio*0.5 + s.fin*0.5, s.fin-s.inicio});
    return BiotSavartSolver(exclusion).calcularCampo(segmentos, corriente, punto);
}
} // namespace magnesim
