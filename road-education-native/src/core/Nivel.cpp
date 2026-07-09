#include "Nivel.h"
#include "../utils/Validador.h"

namespace RoadEducation {

// Valida si una acción del jugador respeta las reglas de seguridad vial
bool Nivel::validarRegla(const std::string& accion) {
    if (!Validador::validarCadena(accion)) return false;

    if (accion == "cruzar_en_verde")      return true;
    if (accion == "usar_paso_peatonal")   return true;
    if (accion == "cruzar_en_rojo")       return false;
    if (accion == "cruzar_fuera_de_paso") return false;

    return false; // acción desconocida: se considera inválida por seguridad
}

// Construye el escenario correspondiente a un nivel.
// En una versión posterior esto puede leerse desde un archivo de configuración.
Escenario Nivel::cargarEscenario(int id) {
    if (!Validador::validarIdNivel(id)) {
        return Escenario(0, "invalido");
    }

    Escenario esc(id, "Nivel " + std::to_string(id));

    if (id == 1) {
        esc.agregarObstaculo(Obstaculo(100.0f, 200.0f, 40.0f, 40.0f, "auto"));
        esc.agregarObstaculo(Obstaculo(300.0f, 150.0f, 40.0f, 40.0f, "semaforo"));
    } else if (id == 2) {
        esc.agregarObstaculo(Obstaculo(150.0f, 100.0f, 50.0f, 50.0f, "bicicleta"));
        esc.agregarObstaculo(Obstaculo(400.0f, 250.0f, 40.0f, 40.0f, "auto"));
        esc.agregarObstaculo(Obstaculo(500.0f, 300.0f, 60.0f, 40.0f, "bus"));
    }

    return esc;
}

// El nivel se considera completado cuando ya no quedan obstáculos activos
bool Nivel::esCompletado(const Escenario& esc) {
    return esc.obstaculosActivos().empty();
}

} // namespace RoadEducation
