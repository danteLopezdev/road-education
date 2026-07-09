#include "Validador.h"

namespace RoadEducation {

bool Validador::validarRango(float val, float min, float max) {
    return val >= min && val <= max;
}

bool Validador::validarIdNivel(int id) {
    return id >= 1;
}

bool Validador::validarCadena(const std::string& cadena) {
    return !cadena.empty();
}

}
