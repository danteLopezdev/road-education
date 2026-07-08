#include <string>
#include "../entidades/Escenario.h"

class Nivel {
public:
    bool      validarRegla(const std::string& accion);
    Escenario cargarEscenario(int id);
    bool      esCompletado(const Escenario& esc);
};
