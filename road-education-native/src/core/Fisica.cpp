#include "Fisica.h"
#include "../utils/Constantes.h"

namespace RoadEducation {

// Actualiza la posición del jugador según su velocidad y el delta de tiempo
void Fisica::calcularPosicion(Jugador& j, float delta) {
    j.x += j.velX * delta;
    j.y += j.velY * delta;
}

// Aplica gravedad de forma continua sobre la velocidad vertical.
// El aterrizaje (poner enSuelo = true y frenar la caída en velY = 0)
// se resuelve aparte, cuando Colisiones detecta contacto con el suelo
// u otra superficie — no aquí, para no acoplar física con colisiones.
void Fisica::aplicarGravedad(Jugador& j) {
    j.velY += GRAVEDAD * (1.0f / FPS_TARGET);
}

// v = v0 + a * t
float Fisica::calcularVelocidad(float vel, float acel, float delta) {
    return vel + acel * delta;
}

} // namespace RoadEducation
