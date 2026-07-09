#ifndef COLISIONES_H
#define COLISIONES_H

#include "../entidades/Jugador.h"
#include "../entidades/Obstaculo.h"
#include "../entidades/Escenario.h"

namespace RoadEducation {

// Lógica pura de C++, sin dependencia de Node/napi.h
class Colisiones {
public:
    bool detectarAABB(const Jugador& a, const Obstaculo& b);
    bool estaEnZona(float x, float y, const Escenario& zona);
};

} // namespace RoadEducation

#endif // COLISIONES_H
