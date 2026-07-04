#include "Fisica.h"
#include "../utils/Constantes.h"

void Fisica::calcularPosicion(Jugador& j, float delta) {
    j.x += j.velX * delta;
    j.y += j.velY * delta;
}

void Fisica::aplicarGravedad(Jugador& j) {
    if (!j.enSuelo) {
        j.velY += RoadEducation::GRAVEDAD * (1.0f / RoadEducation::FPS_TARGET);
    } else {
        j.velY = 0.0f;
    }
}

float Fisica::calcularVelocidad(float vel, float acel, float delta) {
    return vel + acel * delta;
}
