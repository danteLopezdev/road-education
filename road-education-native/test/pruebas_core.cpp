#include <iostream>
#include "../src/entidades/Jugador.h"
#include "../src/entidades/Obstaculo.h"
#include "../src/entidades/Escenario.h"
#include "../src/utils/Constantes.h"
#include "../src/utils/Validador.h"
using namespace std; // Para poder usar cout y endl directamente como en tu main.cpp

int main() {
    cout << "==== PRUEBAS DEL CORE ====" << endl;
    
    // Creamos las entidades
    Jugador p(100.0f, 50.0f);
    Obstaculo obs(200.0f, 50.0f, 30.0f, 30.0f, "SEMAFORO_ROJO");
    Escenario nivel(1, "Nivel Basico");
    
    nivel.agregarObstaculo(obs);

    // Mostramos resultados
    cout << "Vidas del jugador: " << p.vidas << endl;
    cout << "Obstaculo registrado: " << obs.tipo << endl;
    
    if (esPosicionValida(p.x, p.y)) {
        cout << "La posicion del jugador es valida." << endl;
    } else {
        cout << "Error: posicion invalida." << endl;
    }

    return 0;
}