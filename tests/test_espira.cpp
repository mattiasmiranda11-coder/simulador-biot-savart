// tests/test_espira.cpp - MagneSim, Hito 1, T2.2.1 (Benjamin)
// Compara el campo B de una espira circular, calculado con el codigo del equipo,
// contra la formula exacta, usando cada vez mas segmentos.
// No modifica nada de src/.

#include <iostream>
#include <vector>
#include <cmath>

#include "Vector3D.hpp"          // Freddy
#include "EspiraCircular.hpp"    // Mattias
#include "BiotSavartSolver.hpp"  // Freddy

using namespace std;

class BenchmarkEspira {
public:
    BenchmarkEspira(double i_radio, double i_corriente);
    bool ejecutar();

private:
    double radio;
    double corriente;
    double mu0;
    double tolerancia;
    vector<int> listaN;
    vector<double> alturasZ;
    vector<double> erroresCentro;
    bool pasoSimetria;
    bool pasoConvergencia;
    bool pasoCentro;
    bool pasoEje;

    double campoExactoCentro();
    double campoExactoEje(double z);
    Vector3D calcularBNumerico(int nSegmentos, Vector3D punto);
    double absoluto(double x);
    double errorRelativo(double numerico, double exacto);
    void verificarCentro();
    void verificarEje();
    void mostrarResumen();
};

BenchmarkEspira::BenchmarkEspira(double i_radio, double i_corriente)
    : radio(i_radio),
      corriente(i_corriente),
      mu0(4.0 * 3.14159265358979 * 0.0000001),
      tolerancia(0.0001),
      pasoSimetria(false),
      pasoConvergencia(false),
      pasoCentro(false),
      pasoEje(false) {

    int n = 4;
    for (int i = 0; i < 9; i++) {
        listaN.push_back(n);   // 4, 8, 16, ... 1024
        n = n * 2;
    }

    alturasZ.push_back(0.5 * radio);
    alturasZ.push_back(1.0 * radio);
    alturasZ.push_back(2.0 * radio);
}

// B en el centro de la espira: mu0 * I / (2R)
double BenchmarkEspira::campoExactoCentro() {
    return mu0 * corriente / (2.0 * radio);
}

// B sobre el eje, a altura z: mu0 * I * R^2 / (2 * (R^2 + z^2)^(3/2))
double BenchmarkEspira::campoExactoEje(double z) {
    double d2 = radio * radio + z * z;
    double d3 = d2 * sqrt(d2);
    return mu0 * corriente * radio * radio / (2.0 * d3);
}

// UNICO metodo que usa las clases de Mattias y Freddy.
// Si sus nombres o parametros cambian, solo hay que editar estas 3 lineas.
// Supuesto: espira en el plano XY, centrada en el origen, corriente antihoraria (B en +z).
Vector3D BenchmarkEspira::calcularBNumerico(int nSegmentos, Vector3D punto) {
    EspiraCircular espira(radio, corriente, nSegmentos);
    BiotSavartSolver solver;
    return solver.calcularCampo(espira, punto);
}

double BenchmarkEspira::absoluto(double x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

double BenchmarkEspira::errorRelativo(double numerico, double exacto) {
    return absoluto(numerico - exacto) / absoluto(exacto);
}

void BenchmarkEspira::verificarCentro() {
    cout << "--- 1) Campo en el centro de la espira ---" << endl;
    double exacto = campoExactoCentro();
    cout << "B exacto = mu0*I/(2R) = " << exacto << " T" << endl;

    Vector3D origen(0.0, 0.0, 0.0);
    int total = listaN.size();
    pasoSimetria = true;
    erroresCentro.clear();

    for (int i = 0; i < total; i++) {
        int n = listaN[i];
        Vector3D B = calcularBNumerico(n, origen);
        double err = errorRelativo(B.z, exacto);
        erroresCentro.push_back(err);

        cout << "N = " << n << "   B = " << B.z << " T   error = " << err;
        if (i > 0) {
            // si el codigo esta bien, al doblar N este cociente se acerca a 4
            cout << "   (error anterior / error = " << erroresCentro[i - 1] / err << ")";
        }
        cout << endl;

        // en el centro, Bx y By deben ser ~0 por simetria
        if (absoluto(B.x) > 0.000000001 * exacto || absoluto(B.y) > 0.000000001 * exacto) {
            pasoSimetria = false;
        }
    }

    pasoConvergencia = true;
    for (int i = 1; i < total; i++) {
        if (erroresCentro[i] > erroresCentro[i - 1]) {
            pasoConvergencia = false;
        }
    }

    pasoCentro = (erroresCentro[total - 1] < tolerancia);
}

void BenchmarkEspira::verificarEje() {
    cout << endl << "--- 2) Campo sobre el eje de la espira (N mas fino) ---" << endl;
    int nFino = listaN[listaN.size() - 1];
    pasoEje = true;

    for (int i = 0; i < 3; i++) {
        double z = alturasZ[i];
        double exacto = campoExactoEje(z);
        Vector3D B = calcularBNumerico(nFino, Vector3D(0.0, 0.0, z));
        double err = errorRelativo(B.z, exacto);

        cout << "z = " << z << " m   exacto = " << exacto << " T   numerico = " << B.z
             << " T   error = " << err << endl;

        if (err >= tolerancia) {
            pasoEje = false;
        }
    }
}

void BenchmarkEspira::mostrarResumen() {
    cout << endl << "--- RESUMEN (tolerancia = " << tolerancia << ") ---" << endl;
    cout << "Simetria en el centro (Bx, By = 0)  : ";
    if (pasoSimetria) cout << "PASA" << endl; else cout << "FALLA" << endl;
    cout << "Error baja al aumentar N            : ";
    if (pasoConvergencia) cout << "PASA" << endl; else cout << "FALLA" << endl;
    cout << "Error en el centro < tolerancia     : ";
    if (pasoCentro) cout << "PASA" << endl; else cout << "FALLA" << endl;
    cout << "Error sobre el eje < tolerancia     : ";
    if (pasoEje) cout << "PASA" << endl; else cout << "FALLA" << endl;
}

bool BenchmarkEspira::ejecutar() {
    cout << "MagneSim - Benchmark de espira circular (T2.2.1)" << endl;
    cout << "R = " << radio << " m, I = " << corriente << " A" << endl << endl;

    verificarCentro();
    verificarEje();
    mostrarResumen();

    bool todoOk = pasoSimetria && pasoConvergencia && pasoCentro && pasoEje;
    cout << endl;
    if (todoOk) {
        cout << "RESULTADO GLOBAL: PASA" << endl;
    } else {
        cout << "RESULTADO GLOBAL: FALLA" << endl;
    }
    return todoOk;
}

int main() {
    BenchmarkEspira benchmark(0.10, 1.0);   // R = 10 cm, I = 1 A
    bool ok = benchmark.ejecutar();
    if (ok) {
        return 0;
    }
    return 1;
}
