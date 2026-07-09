#ifndef ROAD_EDUCATION_CONSTANTES_H
#define ROAD_EDUCATION_CONSTANTES_H

// Constantes.h
// Valores fijos usados por todo el addon (entidades, core y bindings).
// Este archivo NO tiene .cpp: son constantes de compilacion (constexpr).

namespace RoadEducation {

    // Física
    constexpr float GRAVEDAD = 980.0f;      // px/s^2 (aprox. gravedad "de juego")

    // Reglas del juego
    constexpr int MAX_VIDAS = 3;

    // Rendimiento
    constexpr int FPS_TARGET = 60;

    // Pantalla / mundo del juego
    constexpr int PANTALLA_ANCHO = 800;
    constexpr int PANTALLA_ALTO  = 600;

}

#endif // ROAD_EDUCATION_CONSTANTES_H
