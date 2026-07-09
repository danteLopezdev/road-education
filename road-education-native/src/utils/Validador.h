#ifndef ROAD_EDUCATION_VALIDADOR_H
#define ROAD_EDUCATION_VALIDADOR_H

#include <string>

// Validador.h
// Valida rangos numericos, IDs de nivel y cadenas ANTES de que los datos
// que llegan desde JS (via los bindings de Dante) toquen el core de Lutber.
// No depende de napi.h ni de Node: es C++ puro, igual que entidades/ y core/.

namespace RoadEducation {

class Validador {
public:
    // Verifica que val este dentro de [min, max] (inclusive).
    static bool validarRango(float val, float min, float max);

    // Verifica que el id de nivel sea valido (por ahora: entero positivo).
    // TODO: si se define una cantidad fija de niveles, acotar el limite superior aqui.
    static bool validarIdNivel(int id);

    // Verifica que la cadena no venga vacia (ej: nombre de escenario, tipo de obstaculo).
    static bool validarCadena(const std::string& cadena);
};

}

#endif // ROAD_EDUCATION_VALIDADOR_H
