#include "Puntaje.h"

namespace RoadEducation {

Puntaje::Puntaje() : puntos(0), nivel(1), puntosParaSubirNivel(100) {}

// Suma puntos y sube de nivel automáticamente si se alcanza el umbral
void Puntaje::sumar(int n) {
    puntos += n;
    if (puntos >= nivel * puntosParaSubirNivel) {
        nivel++;
    }
}

// Penalización al perder una vida (colisión con obstáculo, error de regla, etc.)
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

} // namespace RoadEducation
