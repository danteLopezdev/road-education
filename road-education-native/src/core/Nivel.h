#ifndef NIVEL_H
#define NIVEL_H

#include <string>
#include "../entidades/Escenario.h"

namespace RoadEducation {

// Lógica pura de C++, sin dependencia de Node/napi.h
class Nivel {
public:
    bool      validarRegla(const std::string& accion);
    Escenario cargarEscenario(int id);
    bool      esCompletado(const Escenario& esc);
};

} // namespace RoadEducation

#endif // NIVEL_H
