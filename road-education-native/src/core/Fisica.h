#ifndef FISICA_H
#define FISICA_H

#include "../entidades/Jugador.h"

namespace RoadEducation {

// Lógica pura de C++, sin dependencia de Node/napi.h
class Fisica {
public:
    void  calcularPosicion(Jugador& j, float delta);
    void  aplicarGravedad(Jugador& j);
    float calcularVelocidad(float vel, float acel, float delta);
};

} // namespace RoadEducation

#endif // FISICA_H
