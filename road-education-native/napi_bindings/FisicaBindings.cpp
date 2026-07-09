#include "FisicaBindings.h"
#include "../src/core/Fisica.h"
#include "JugadorWrap.h"

using namespace RoadEducation;

Napi::Object FisicaBindings::Init(Napi::Env env, Napi::Object exports) {
    exports.Set("calcularPosicion", Napi::Function::New(env, CalcularPosicion));
    exports.Set("aplicarGravedad", Napi::Function::New(env, AplicarGravedad));
    exports.Set("calcularVelocidad", Napi::Function::New(env, CalcularVelocidad));
    return exports;
}

// JS: calcularPosicion(jugadorNativo, delta) -- muta el jugador in-place
Napi::Value FisicaBindings::CalcularPosicion(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "calcularPosicion(jugadorNativo, delta) espera (objeto, numero)").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    JugadorWrap* jw = Napi::ObjectWrap<JugadorWrap>::Unwrap(info[0].As<Napi::Object>());
    float delta = info[1].As<Napi::Number>().FloatValue();

    Fisica().calcularPosicion(jw->Get(), delta);
    return env.Undefined();
}

// JS: aplicarGravedad(jugadorNativo) -- muta el jugador in-place
Napi::Value FisicaBindings::AplicarGravedad(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsObject()) {
        Napi::TypeError::New(env, "aplicarGravedad(jugadorNativo) espera un JugadorNativo").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    JugadorWrap* jw = Napi::ObjectWrap<JugadorWrap>::Unwrap(info[0].As<Napi::Object>());
    Fisica().aplicarGravedad(jw->Get());
    return env.Undefined();
}

// JS: calcularVelocidad(vel, acel, delta) -> number
Napi::Value FisicaBindings::CalcularVelocidad(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 3 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber()) {
        Napi::TypeError::New(env, "calcularVelocidad(vel, acel, delta) espera 3 numeros").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    float vel = info[0].As<Napi::Number>().FloatValue();
    float acel = info[1].As<Napi::Number>().FloatValue();
    float delta = info[2].As<Napi::Number>().FloatValue();

    float resultado = Fisica().calcularVelocidad(vel, acel, delta);
    return Napi::Number::New(env, resultado);
}
