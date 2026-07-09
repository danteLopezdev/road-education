#ifndef NAPI_NIVEL_BINDINGS_H
#define NAPI_NIVEL_BINDINGS_H

#include <napi.h>

// NivelBindings
// RoadEducation::Nivel no tiene estado propio: se expone como funciones
// sueltas del modulo.
// Responsable: Dante
//
// Firmas JS previstas (ver ../src/core/Nivel.h):
//   validarRegla(accion: string)                -> boolean
//   cargarEscenario(id: number)                 -> EscenarioNativo
//   esCompletado(escenarioNativo)               -> boolean
//
// cargarEscenario es el mas dificil de los tres: depende de que
// EscenarioWrap::NewInstance este implementado primero (ver EscenarioWrap.h).
namespace NivelBindings {
    Napi::Object Init(Napi::Env env, Napi::Object exports);

    Napi::Value ValidarRegla(const Napi::CallbackInfo& info);
    Napi::Value CargarEscenario(const Napi::CallbackInfo& info);
    Napi::Value EsCompletado(const Napi::CallbackInfo& info);
}

#endif // NAPI_NIVEL_BINDINGS_H
