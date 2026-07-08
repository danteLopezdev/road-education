#include "Puntaje.h"

Puntaje::Puntaje() : puntos(0), nivel(1), puntosParaSubirNivel(100) {}

void Puntaje::sumar(int n) {
    puntos += n;
    if (puntos >= nivel * puntosParaSubirNivel) {
        nivel++;
    }
}

void Puntaje::restarVida() {
    puntos -= 10;
    if (puntos < 0) {
        puntos = 0;
    }
}

int Puntaje::getNivel() const {
    return nivel;
}

int Puntaje::getPuntos() const {
    return puntos;
}

void Puntaje::reset() {
    puntos = 0;
    nivel = 1;
}
