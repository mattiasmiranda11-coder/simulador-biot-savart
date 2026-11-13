/* Compara el campo de una espira circular, calculado con el codigo del equipo,
contra la formula exacta, usando cada vez mas segmentos*/
#include <iostream>
#include <vector>
#include <cmath>

#include "biot_savart.hpp"   /*Freddy*/
#include "EspiraCircular.hpp"  /*Matty*/

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
    vector<magnesim::Segmento> generarEspira(int nSegmentos);
    magnesim::Vec3 calcularBNumerico(int nSegmentos, magnesim::Vec3 punto);
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
        listaN.push_back(n);  
        n = n * 2;
    }
    
    alturasZ.push_back(0.5 * radio);
    alturasZ.push_back(1.0 * radio);
    alturasZ.push_back(2.0 * radio);
}

/*B en el centro de la espira*/
double BenchmarkEspira::campoExactoCentro() {
    return mu0 * corriente / (2.0 * radio);
}

/* B sobre el eje, a cierta alturaz.*/
double BenchmarkEspira::campoExactoEje(double z) {
    double d2 = radio * radio + z * z;
    double d3 = d2 * sqrt(d2);
    return mu0 * corriente * radio * radio / (2.0 * d3);
}

/* pidelos segmentos a EspiraCircular de Mattias y los convierte al formato del solver de Freddy*/
vector<magnesim::Segmento> BenchmarkEspira::generarEspira(int nSegmentos) {
    EspiraCircular espira(corriente, radio, nSegmentos);
    vector<Segmento> segmentos = espira.generarSegmentos(nSegmentos);
    
    vector<magnesim::Segmento> lista;
    for (size_t i = 0; i < segmentos.size(); i++) {
        magnesim::Segmento seg;
        seg.inicio = {segmentos[i].centro.x - 0.5 * segmentos[i].dl.x,
                      segmentos[i].centro.y - 0.5 * segmentos[i].dl.y,
                      segmentos[i].centro.z - 0.5 * segmentos[i].dl.z};
        seg.fin = {segmentos[i].centro.x + 0.5 * segmentos[i].dl.x,
                   segmentos[i].centro.y + 0.5 * segmentos[i].dl.y,
                   segmentos[i].centro.z + 0.5 * segmentos[i].dl.z};
        lista.push_back(seg);
    }
    return lista;
}

/* unico metodo que usa el solver de Freddy.
calcular_campo devuelve un "optional": si el punt esta demasiado cerca del conductor
(muestra excluida) viene vacio, y aqui lo avisamos y devolvemos cero*/
magnesim::Vec3 BenchmarkEspira::calcularBNumerico(int nSegmentos, magnesim::Vec3 punto) {
    vector<magnesim::Segmento> tramos = generarEspira(nSegmentos);
    std::optional<magnesim::Vec3> resultado = magnesim::calcular_campo(tramos, corriente, punto);
    if (!resultado.has_value()) {
        cout << "AVISO: punto excluido por el solver (muy cerca del conductor)" << endl;
        magnesim::Vec3 cero = {0.0, 0.0, 0.0};
        return cero;
    }
    return resultado.value();
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
    
    magnesim::Vec3 origen = {0.0, 0.0, 0.0};
    int total = listaN.size();
    pasoSimetria = true;
    erroresCentro.clear();
    
    for (int i = 0; i < total; i++) {
        int n = listaN[i];
        magnesim::Vec3 B = calcularBNumerico(n, origen);
        double err = errorRelativo(B.z, exacto);
        erroresCentro.push_back(err);
        
        cout << "N = " << n << "   B = " << B.z << " T   error = " << err;
        if (i > 0) {
            if (err > 0.000000000001) {
                cout << "   (error anterior / error = " << erroresCentro[i - 1] / err << ")";
            } else {
                cout << "   (error de redondeo, ya es exacto)";
            }
        }
        cout << endl;
        
        /*en el centro, Bx y By deben ser 0 o casi 0 por simetria*/
        if (absoluto(B.x) > 0.000000001 * exacto || absoluto(B.y) > 0.000000001 * exacto) {
            pasoSimetria = false;
        }
    }
    
    pasoConvergencia = true;
    for (int i = 1; i < total; i++) {
        /* el error debe bajar; si ya es del tamano del redondeo (< 1e-12) se acepta*/
        if (erroresCentro[i] > erroresCentro[i - 1] && erroresCentro[i] > 0.000000000001) {
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
        magnesim::Vec3 punto = {0.0, 0.0, z};
        magnesim::Vec3 B = calcularBNumerico(nFino, punto);
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
    BenchmarkEspira benchmark(0.10, 1.0); /*asigno radio=0,1m y 1 A de corriente*/
    bool ok = benchmark.ejecutar();
    if (ok) {
        return 0;
    }
    return 1;
}
