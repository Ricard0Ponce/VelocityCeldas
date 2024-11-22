//
// Created by rich on 15/11/24.
//

#ifndef CELDA_H
#define CELDA_H
#include <vector>
using namespace std;

class Celda {
public:
    Celda();
    virtual ~Celda();
    int nac; // Numero de atomos en la celda
    vector <int> iats; // Vector de coordenadas de los atomos
};



#endif //CELDA_H
