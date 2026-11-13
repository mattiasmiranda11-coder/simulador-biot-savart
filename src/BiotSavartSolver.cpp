#include "fisica/BiotSavartSolver.hpp"
#include <algorithm>
#include <stdexcept>

BiotSavartSolver::BiotSavartSolver(double exclusion) : exclusion_(exclusion) {
    if (!std::isfinite(exclusion_) || exclusion_ <= 0)
        throw std::invalid_argument("La exclusion debe ser positiva y finita");
}

std::optional<Vector3D> BiotSavartSolver::calcularCampo(
    const Conductor& conductor, Vector3D punto, int numeroSegmentos) const {
    if (numeroSegmentos <= 0)
        throw std::invalid_argument("Se requiere un numero positivo de segmentos");
    return calcularCampo(conductor.generarSegmentos(numeroSegmentos),
                         conductor.getCorriente(), punto);
}

std::optional<Vector3D> BiotSavartSolver::calcularCampo(
    const std::vector<Segmento>& segmentos, double corriente, Vector3D punto) const {
    if (!punto.finito() || !std::isfinite(corriente) || segmentos.empty())
        throw std::invalid_argument("Punto, corriente o lista de segmentos invalidos");
    // Revisar toda la entrada incluso si un segmento excluye el punto.
    for (const auto& s : segmentos) {
        const double longitud2 = s.dl.punto(s.dl);
        if (!s.centro.finito() || !s.dl.finito() ||
            !std::isfinite(longitud2) || longitud2 <= 0)
            throw std::invalid_argument("Segmento no finito, degenerado o fuera de escala");
    }
    Vector3D campo;
    const double factor = 1e-7 * corriente; // mu0/(4*pi) * I, aproximacion SI.
    for (const auto& s : segmentos) {
        const Vector3D r = punto - s.centro;
        // Distancia al segmento recto finito: centro-dl/2 hasta centro+dl/2.
        const double proyeccion = r.punto(s.dl) / s.dl.punto(s.dl);
        if (!r.finito() || !std::isfinite(proyeccion))
            throw std::overflow_error("Coordenadas fuera de escala");
        const double t = std::clamp(proyeccion, -0.5, 0.5);
        if ((r - s.dl*t).norma() <= exclusion_) return std::nullopt;
        const double distancia = r.norma();
        if (!std::isfinite(distancia) || distancia <= 0)
            throw std::overflow_error("Distancia fuera de escala");
        // Regla del punto medio: dB = mu0*I/(4*pi) * (dl x r_unitario)/r^2.
        campo = campo + s.dl.cruz(r*(1.0/distancia))
                         * (factor/distancia/distancia);
    }
    if (!campo.finito()) throw std::overflow_error("Campo fuera de escala");
    return campo;
}
