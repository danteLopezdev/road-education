#ifndef NAPI_COLISIONES_BINDINGS_H
#define NAPI_COLISIONES_BINDINGS_H

#include <napi.h>

// ColisionesBindings
// RoadEducation::Colisiones no tiene estado propio: se expone como
// funciones sueltas del modulo.
// Responsable: Dante
//
// Firmas JS previstas (ver ../src/core/Colisiones.h):
//   detectarAABB(jugadorNativo, obstaculoNativo)   -> boolean
//   estaEnZona(x, y, escenarioNativo)              -> boolean
namespace ColisionesBindings {
    Napi::Object Init(Napi::Env env, Napi::Object exports);

    // TODO(Dante): implementar. Unwrap info[0] (JugadorWrap) e info[1]
    // (ObstaculoWrap), llamar Colisiones().detectarAABB(jw->Get(), ow->Get()).
    Napi::Value DetectarAABB(const Napi::CallbackInfo& info);

    // TODO(Dante): leer x, y (numbers), unwrap info[2] (EscenarioWrap),
    // llamar Colisiones().estaEnZona(x, y, ew->Get()).
    Napi::Value EstaEnZona(const Napi::CallbackInfo& info);
}

#endif // NAPI_COLISIONES_BINDINGS_H
