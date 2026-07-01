#pragma once
#include <vector>
#include <string>
#include "Obstaculo.h"
using namespace std;

class Escenario {
public:
    int idNivel;
    string nombre;
    vector<Obstaculo> obstaculos; // Usamos vector en lugar de arreglos por exigencia de memoria dinámica

    Escenario(int id, string nombreNivel);
    void agregarObstaculo(Obstaculo obs);
};