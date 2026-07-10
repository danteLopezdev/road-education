#ifndef ROAD_EDUCATION_OBSTACULO_H
#define ROAD_EDUCATION_OBSTACULO_H

#include <string>
#include "Jugador.h"

// Obstaculo.h
// Entidad de dominio: senal de trafico, hueco, vehiculo, etc. (segun "tipo").

namespace RoadEducation {

class Obstaculo {
public:
    float x, y, ancho, alto;
    std::string tipo;
    bool activo;

    Obstaculo(float x = 0.0f, float y = 0.0f,
              float ancho = 0.0f, float alto = 0.0f,
              std::string tipo = "generico");

    // Chequeo AABB simple a nivel de entidad (jugador tratado como punto x,y).
    // Es un helper de conveniencia/pruebas locales; la deteccion "oficial"
    // que usa el juego vive en core/Colisiones::detectarAABB(), a cargo de Lutber.
    bool colisionaCon(const Jugador& jugador) const;

    void activar();
    void desactivar();

    // Desplaza el obstaculo (uso principal: autos en carriles, movimiento
    // continuo en pixeles por frame). Simetrico a Jugador::mover.
    void mover(float dx, float dy);
};

}

#endif // ROAD_EDUCATION_OBSTACULO_H
