#include "Validador.h"
#include "Constantes.h"

bool esPosicionValida(float x, float y) {
    if (x >= 0.0f && x <= PANTALLA_ANCHO && y >= 0.0f && y <= PANTALLA_ALTO) {
        return true;
    }
    return false;
}

bool esIdNivelValido(int id) {
    return id > 0;
}

bool esCadenaValida(string str) {
    return str != "";
}