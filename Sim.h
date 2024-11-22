//
// Created by rich on 25/10/24.
//

#ifndef SIM_H
#define SIM_H
#include <vector>
#include <time.h>
#include "Atomo.h"
#include "Celda.h"
using namespace std;
#include <iostream>



class Sim {
public:
    Sim();
    virtual ~Sim();
    // Variables globales
    int na, nd, nc, ncp, nci, ncvc;
    double dt, L, d, r, v0, temp, dens, rc, drc , rcd;
    double dis[3], f;
    string cuad, metodo;
    vector <Atomo*> atomos;
    vector <Celda*> celdas;
    int nct, nctt[3], nrc;
    double et, ec, ei;
    void calcularVC();
    int prin();
    void Datos();
    void EscribirDatos();
    void IniciarCel();
    void CalcularCel();

    void IniciarAtomos();
    void EscribirPosVel();
    void Prop();

    void Simulacion();
    void Aceleraciones();
    double LJ(double r);
    double Dist (int i, int j);
    void Cuadrada();
    void AceleracionesVC();
    void AceleracionesCel();


protected:

private:



    /*
    ~Sim();

    int na = 100; // Numero de atomos
    int nd = 3; // Numero de dimensiones
    int nc = 1000; // Numero de configuraciones

    double dt = 0.1; // Delta de tiempo
    double L = 1.0; // Espacio
    double d= 0.1;
    double r = d / 2.0;
    double v0 = 1;

    int ncp =10;// Numero de configuraciones parciales
    int nci = nc / ncp;

    double p[100][100], v[100][100]; // Matrices
    */


};



#endif //SIM_H
