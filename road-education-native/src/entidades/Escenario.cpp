#include "Escenario.h"

Escenario::Escenario(int id, string nombreNivel) {
    idNivel = id;
    nombre = nombreNivel;
}

void Escenario::agregarObstaculo(Obstaculo obs) {
    obstaculos.push_back(obs);
}