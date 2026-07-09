// pruebas_core.cpp
// Pruebas unitarias compiladas SIN Node.js (no incluyen napi.h).
// Compilar con:
//   g++ -std=c++17 test/pruebas_core.cpp src/core/*.cpp src/entidades/*.cpp -o test_runner && ./test_runner
//
// Responsable: Marlon
//
// IMPORTANTE: la seccion "Pruebas de core" asume que Lutber ya entrego
// src/core/Fisica.h, Colisiones.h, Puntaje.h y Nivel.h con las firmas
// definidas en las Especificaciones Tecnicas (seccion 4.3). Si Lutber
// ajusta alguna firma o el comportamiento interno, hay que revisar estas
// pruebas (sobre todo testNivel, cuyas reglas de negocio aun no estan
// definidas del todo).

#include <cassert>
#include <iostream>
#include <string>

#include "../src/entidades/Jugador.h"
#include "../src/entidades/Obstaculo.h"
#include "../src/entidades/Escenario.h"

#include "../src/core/Fisica.h"
#include "../src/core/Colisiones.h"
#include "../src/core/Puntaje.h"
#include "../src/core/Nivel.h"

using namespace RoadEducation;

static int pruebasEjecutadas = 0;
static int pruebasFallidas   = 0;

void verificar(bool condicion, const std::string& nombrePrueba) {
    pruebasEjecutadas++;
    if (condicion) {
        std::cout << "[OK]   " << nombrePrueba << std::endl;
    } else {
        pruebasFallidas++;
        std::cout << "[FAIL] " << nombrePrueba << std::endl;
    }
}

// ------------------------------------------------------------------
// Pruebas de entidades (Marlon) — no dependen de Lutber, ya funcionan
// ------------------------------------------------------------------

void testJugador() {
    Jugador j(10.0f, 20.0f);
    verificar(j.x == 10.0f && j.y == 20.0f, "Jugador se crea en la posicion indicada");
    verificar(j.vidas == MAX_VIDAS, "Jugador inicia con MAX_VIDAS");

    j.mover(5.0f, -3.0f);
    verificar(j.x == 15.0f && j.y == 17.0f, "Jugador::mover desplaza x,y correctamente");

    j.perderVida();
    verificar(j.vidas == MAX_VIDAS - 1, "Jugador::perderVida resta una vida");
    verificar(j.estaVivo(), "Jugador sigue vivo con vidas > 0");

    for (int i = 0; i < MAX_VIDAS; i++) j.perderVida();
    verificar(!j.estaVivo(), "Jugador muere al llegar a 0 vidas");
    verificar(j.vidas == 0, "Jugador::perderVida no resta por debajo de 0");

    j.resetPosicion();
    verificar(j.x == 10.0f && j.y == 20.0f, "Jugador::resetPosicion regresa a la posicion inicial");
}

void testObstaculo() {
    Obstaculo o(0.0f, 0.0f, 50.0f, 50.0f, "senal_stop");
    verificar(o.activo, "Obstaculo inicia activo");

    o.desactivar();
    verificar(!o.activo, "Obstaculo::desactivar funciona");

    o.activar();
    verificar(o.activo, "Obstaculo::activar funciona");

    Jugador dentro(10.0f, 10.0f);
    Jugador fuera(200.0f, 200.0f);

    verificar(o.colisionaCon(dentro), "Obstaculo detecta colision con jugador dentro del area");
    verificar(!o.colisionaCon(fuera), "Obstaculo NO detecta colision con jugador fuera del area");
}

void testEscenario() {
    Escenario esc(1, "Cruce Escolar");
    verificar(esc.idNivel == 1 && esc.nombre == "Cruce Escolar", "Escenario se crea con id y nombre");

    esc.agregarObstaculo(Obstaculo(0, 0, 10, 10, "cono"));
    esc.agregarObstaculo(Obstaculo(20, 20, 10, 10, "bache"));
    verificar(esc.obstaculos.size() == 2, "Escenario::agregarObstaculo agrega obstaculos");

    esc.obstaculos[1].desactivar();
    verificar(esc.obstaculosActivos().size() == 1, "Escenario::obstaculosActivos filtra los inactivos");

    esc.limpiar();
    verificar(esc.obstaculos.empty(), "Escenario::limpiar vacia la lista de obstaculos");
}

// ------------------------------------------------------------------
// Pruebas de core (Lutber) — requieren src/core/*.h y *.cpp ya implementados
// ------------------------------------------------------------------

void testFisica() {
    Fisica fisica;
    Jugador j(0.0f, 0.0f);
    j.velY = 10.0f;

    fisica.calcularPosicion(j, 1.0f);
    verificar(j.y != 0.0f, "Fisica::calcularPosicion mueve al jugador segun su velocidad");

    float velInicial = j.velY;
    fisica.aplicarGravedad(j);
    verificar(j.velY > velInicial, "Fisica::aplicarGravedad incrementa la velocidad vertical");

    float nuevaVel = fisica.calcularVelocidad(5.0f, 2.0f, 1.0f);
    verificar(nuevaVel == 7.0f, "Fisica::calcularVelocidad = vel + acel*delta");
}

void testColisiones() {
    Colisiones colisiones;
    Jugador j(5.0f, 5.0f);
    Obstaculo o(0.0f, 0.0f, 10.0f, 10.0f, "hueco");

    verificar(colisiones.detectarAABB(j, o), "Colisiones::detectarAABB detecta una colision real");

    Jugador lejos(500.0f, 500.0f);
    verificar(!colisiones.detectarAABB(lejos, o), "Colisiones::detectarAABB no detecta cuando no hay colision");
}

void testPuntaje() {
    Puntaje puntaje;
    puntaje.reset();
    puntaje.sumar(10);
    puntaje.sumar(5);
    // No hay getter de puntos totales en la especificacion, solo getNivel().
    verificar(puntaje.getNivel() >= 0, "Puntaje::getNivel devuelve un nivel valido tras sumar puntos");

    puntaje.restarVida();
    verificar(true, "Puntaje::restarVida se ejecuta sin errores"); // placeholder hasta tener mas API
}

void testNivel() {
    // NOTA: estas dos aserciones son orientativas; ajustar cuando Lutber
    // defina las reglas reales de "validarRegla" y "esCompletado".
    Nivel nivel;
    verificar(nivel.validarRegla("cruzar_en_verde"), "Nivel::validarRegla acepta una accion valida");

    Escenario esc = nivel.cargarEscenario(1);
    verificar(esc.idNivel == 1, "Nivel::cargarEscenario devuelve el escenario solicitado");

    verificar(!nivel.esCompletado(esc), "Nivel::esCompletado es false en un escenario recien cargado");
}

int main() {
    std::cout << "=== Pruebas de entidades (Marlon) ===" << std::endl;
    testJugador();
    testObstaculo();
    testEscenario();

    std::cout << "\n=== Pruebas de core (Lutber) ===" << std::endl;
    testFisica();
    testColisiones();
    testPuntaje();
    testNivel();

    std::cout << "\n" << pruebasEjecutadas << " pruebas ejecutadas, "
              << pruebasFallidas << " fallidas." << std::endl;

    return pruebasFallidas == 0 ? 0 : 1;
}
