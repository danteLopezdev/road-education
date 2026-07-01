#pragma once

class Jugador {
public:
    float x, y;
    float velX, velY;
    int vidas;
    bool enSuelo;

    Jugador(float startX, float startY);
};