#ifndef ROAD_EDUCATION_JUGADOR_H
#define ROAD_EDUCATION_JUGADOR_H

#include "../utils/Constantes.h"

// Jugador.h
// Entidad de dominio: representa al personaje controlado en el juego.
// Atributos publicos, sin dependencia de napi.h / Node (esto lo usa el core de Lutber).

namespace RoadEducation {

class Jugador {
public:
    float x, y;
    float velX, velY;
    int   vidas;
    bool  enSuelo;

    // xInicial/yInicial: posicion de spawn, usada por resetPosicion().
    explicit Jugador(float xInicial = 0.0f, float yInicial = 0.0f);

    void mover(float dx, float dy);
    void perderVida();
    bool estaVivo() const;
    void resetPosicion();

    // Reinicio completo tras fin de partida (vidas en 0): restaura posicion
    // (via resetPosicion) y ademas repone las vidas a MAX_VIDAS.
    void resetCompleto();

private:
    float xOrigen, yOrigen;
};

}

#endif // ROAD_EDUCATION_JUGADOR_H
