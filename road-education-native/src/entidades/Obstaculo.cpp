#include "Obstaculo.h"
#include <utility>

namespace RoadEducation {

Obstaculo::Obstaculo(float x, float y, float ancho, float alto, std::string tipo)
    : x(x), y(y), ancho(ancho), alto(alto), tipo(std::move(tipo)), activo(true) {}

bool Obstaculo::colisionaCon(const Jugador& jugador) const {
    if (!activo) return false;

    bool dentroX = jugador.x >= x && jugador.x <= (x + ancho);
    bool dentroY = jugador.y >= y && jugador.y <= (y + alto);
    return dentroX && dentroY;
}

void Obstaculo::activar() {
    activo = true;
}

void Obstaculo::desactivar() {
    activo = false;
}

}
