// addon.cpp
// Punto de entrada del addon nativo (N-API).
// Responsable: Dante
//
// Registra todo lo que el modulo expone a JS/Electron:
// - Clases con estado (Jugador, Obstaculo, Escenario, Puntaje) via ObjectWrap.
// - Logica sin estado (Fisica, Colisiones, Nivel) via funciones sueltas.
//
// Cadena de acceso desde el juego (ver README del proyecto):
//   renderer (Canvas 2D) -> ipcRenderer -> preload.js (contextBridge)
//   -> main.js -> require('road-education-native') -> este addon.
//
// TODO(Dante): a medida que se implementen los metodos pendientes en cada
// wrapper, no hace falta tocar este archivo — cada Init() ya expone su
// clase/funciones completas. Si se agrega una clase o binding nuevo,
// registrarlo aqui.

#include <napi.h>

#include "JugadorWrap.h"
#include "ObstaculoWrap.h"
#include "EscenarioWrap.h"
#include "PuntajeWrap.h"
#include "FisicaBindings.h"
#include "ColisionesBindings.h"
#include "NivelBindings.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    // Clases con estado (Napi::ObjectWrap)
    JugadorWrap::Init(env, exports);
    ObstaculoWrap::Init(env, exports);
    EscenarioWrap::Init(env, exports);
    PuntajeWrap::Init(env, exports);

    // Logica sin estado (funciones sueltas)
    FisicaBindings::Init(env, exports);
    ColisionesBindings::Init(env, exports);
    NivelBindings::Init(env, exports);

    return exports;
}

NODE_API_MODULE(road_education_native, InitAll)
