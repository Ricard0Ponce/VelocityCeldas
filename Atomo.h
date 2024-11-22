//
// Created by rich on 25/10/24.
//

#ifndef ATOMO_H
#define ATOMO_H
#include <vector>

#include <iostream>

using namespace std;

class Atomo {
public:
    Atomo(); // Declaración del constructor

    //std::vector<double> p; // Posiciones
    //std::vector<double> v; // Velocidades
    virtual ~Atomo();

    vector <double>p,v,a;
    // Cada átomo va a tener la lista de ss vecinos
    int nv; // Número de vecinos de cada atomo
    vector<int> vc; // Vector de vecinos cercanos
    int ncel,ccel[3]; // ncel=Numero de las celdas, ccel = Coordenadas del atomo

protected:

private:


};



#endif //ATOMO_H
