#include "Colisiones.h"

bool Colisiones::detectarAABB(const Jugador& a, const Obstaculo& b) {
    if (!b.activo) return false;

    const float jugadorAncho = 32.0f;
    const float jugadorAlto  = 32.0f;

    bool solapaX = a.x < (b.x + b.ancho) && (a.x + jugadorAncho) > b.x;
    bool solapaY = a.y < (b.y + b.alto)  && (a.y + jugadorAlto)  > b.y;

    return solapaX && solapaY;
}

bool Colisiones::estaEnZona(float x, float y, const Escenario& zona) {
    for (const Obstaculo& obs : zona.obstaculos) {
        if (!obs.activo) continue;

        bool dentroX = x >= obs.x && x <= (obs.x + obs.ancho);
        bool dentroY = y >= obs.y && y <= (obs.y + obs.alto);

        if (dentroX && dentroY) {
            return true;
        }
    }
    return false;
}
