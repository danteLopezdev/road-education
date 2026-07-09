#ifndef NAPI_FISICA_BINDINGS_H
#define NAPI_FISICA_BINDINGS_H

#include <napi.h>

// FisicaBindings
// RoadEducation::Fisica no tiene estado propio, asi que no se envuelve
// como clase: se exponen sus 3 metodos como funciones sueltas del modulo.
// Responsable: Dante
//
// Firmas JS previstas (ver ../src/core/Fisica.h para el detalle):
//   calcularPosicion(jugadorNativo, delta)      -> undefined (muta el jugador)
//   aplicarGravedad(jugadorNativo)              -> undefined (muta el jugador)
//   calcularVelocidad(vel, acel, delta)         -> number
namespace FisicaBindings {
    Napi::Object Init(Napi::Env env, Napi::Object exports);

    // TODO(Dante): implementar. Patron: unwrap info[0] como JugadorWrap*
    // (ver ObstaculoWrap::ColisionaCon para el patron de Unwrap), leer
    // info[1] como float, llamar Fisica().calcularPosicion(jw->Get(), delta).
    Napi::Value CalcularPosicion(const Napi::CallbackInfo& info);
    Napi::Value AplicarGravedad(const Napi::CallbackInfo& info);
    Napi::Value CalcularVelocidad(const Napi::CallbackInfo& info);
}

#endif // NAPI_FISICA_BINDINGS_H
