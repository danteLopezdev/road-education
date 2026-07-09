#ifndef NAPI_PUNTAJE_WRAP_H
#define NAPI_PUNTAJE_WRAP_H

#include <napi.h>
#include "../src/core/Puntaje.h"

// PuntajeWrap
// Envuelve RoadEducation::Puntaje como objeto JS ("PuntajeNativo").
// Responsable: Dante
//
// El mas simple de los cuatro wrappers con estado: sin dependencias de
// otras entidades. Buen candidato para implementar primero.
class PuntajeWrap : public Napi::ObjectWrap<PuntajeWrap> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    explicit PuntajeWrap(const Napi::CallbackInfo& info);

private:
    static Napi::FunctionReference constructor;
    RoadEducation::Puntaje puntaje_;

    Napi::Value GetNivel(const Napi::CallbackInfo& info);
    Napi::Value GetPuntos(const Napi::CallbackInfo& info);

    // TODO(Dante): implementar
    Napi::Value Sumar(const Napi::CallbackInfo& info);
    Napi::Value RestarVida(const Napi::CallbackInfo& info);
    Napi::Value Reset(const Napi::CallbackInfo& info);
};

#endif // NAPI_PUNTAJE_WRAP_H
