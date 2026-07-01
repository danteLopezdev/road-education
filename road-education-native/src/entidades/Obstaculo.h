#pragma once
#include <string>
using namespace std;

class Obstaculo {
public:
    float x, y;
    float ancho, alto;
    string tipo;
    bool activo;

    Obstaculo(float startX, float startY, float w, float h, string tipoSenal);
};