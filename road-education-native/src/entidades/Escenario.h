#ifndef ROAD_EDUCATION_ESCENARIO_H
#define ROAD_EDUCATION_ESCENARIO_H

#include <vector>
#include <string>
#include "Obstaculo.h"

// Escenario.h
// Entidad de dominio: agrupa los obstaculos de un nivel especifico.

namespace RoadEducation {

class Escenario {
public:
    std::vector<Obstaculo> obstaculos;
    int idNivel;
    std::string nombre;

    explicit Escenario(int idNivel = 0, std::string nombre = "");

    void agregarObstaculo(const Obstaculo& obstaculo);
    std::vector<Obstaculo> obstaculosActivos() const;
    void limpiar();
};

}

#endif // ROAD_EDUCATION_ESCENARIO_H
