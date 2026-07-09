#include "Escenario.h"
#include <utility>

namespace RoadEducation {

Escenario::Escenario(int idNivel, std::string nombre)
    : idNivel(idNivel), nombre(std::move(nombre)) {}

void Escenario::agregarObstaculo(const Obstaculo& obstaculo) {
    obstaculos.push_back(obstaculo);
}

std::vector<Obstaculo> Escenario::obstaculosActivos() const {
    std::vector<Obstaculo> activos;
    for (const auto& obs : obstaculos) {
        if (obs.activo) {
            activos.push_back(obs);
        }
    }
    return activos;
}

void Escenario::limpiar() {
    obstaculos.clear();
}

}
