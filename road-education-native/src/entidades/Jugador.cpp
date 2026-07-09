#include "Jugador.h"

namespace RoadEducation {

Jugador::Jugador(float xInicial, float yInicial)
    : x(xInicial), y(yInicial),
      velX(0.0f), velY(0.0f),
      vidas(MAX_VIDAS),
      enSuelo(false),
      xOrigen(xInicial), yOrigen(yInicial) {}

void Jugador::mover(float dx, float dy) {
    x += dx;
    y += dy;
}

void Jugador::perderVida() {
    if (vidas > 0) {
        vidas--;
    }
}

bool Jugador::estaVivo() const {
    return vidas > 0;
}

void Jugador::resetPosicion() {
    x = xOrigen;
    y = yOrigen;
    velX = 0.0f;
    velY = 0.0f;
    enSuelo = false;
}

}
