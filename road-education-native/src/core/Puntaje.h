#ifndef PUNTAJE_H
#define PUNTAJE_H

namespace RoadEducation {

// Lógica pura de C++, sin dependencia de Node/napi.h
class Puntaje {
private:
    int puntos;
    int nivel;
    int puntosParaSubirNivel;

public:
    Puntaje();

    void sumar(int n);
    void restarVida();
    int  getNivel() const;
    int  getPuntos() const;
    void reset();
};

} // namespace RoadEducation

#endif // PUNTAJE_H
