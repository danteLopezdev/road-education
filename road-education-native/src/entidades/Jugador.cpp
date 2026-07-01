#include "Jugador.h"
#include "../utils/Constantes.h"

Jugador::Jugador(float startX, float startY) {
    x = startX;
    y = startY;
    velX = 0.0f;
    velY = 0.0f;
    vidas = MAX_VIDAS;
    enSuelo = false;
}