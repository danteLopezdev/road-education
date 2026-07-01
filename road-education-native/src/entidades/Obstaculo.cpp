#include "Obstaculo.h"

Obstaculo::Obstaculo(float startX, float startY, float w, float h, string tipoSenal) {
    x = startX;
    y = startY;
    ancho = w;
    alto = h;
    tipo = tipoSenal;
    activo = true;
}