#include "../entidades/Jugador.h"
#include "../entidades/Obstaculo.h"
#include "../entidades/Escenario.h"

class Colisiones {
public:
    bool detectarAABB(const Jugador& a, const Obstaculo& b);
    bool estaEnZona(float x, float y, const Escenario& zona);
};
