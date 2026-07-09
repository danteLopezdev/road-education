#include "PuntajeWrap.h"

using namespace RoadEducation;

Napi::FunctionReference PuntajeWrap::constructor;

Napi::Object PuntajeWrap::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "PuntajeNativo", {
        InstanceAccessor("nivel", &PuntajeWrap::GetNivel, nullptr),
        InstanceAccessor("puntos", &PuntajeWrap::GetPuntos, nullptr),
        InstanceMethod("sumar", &PuntajeWrap::Sumar),
        InstanceMethod("restarVida", &PuntajeWrap::RestarVida),
        InstanceMethod("reset", &PuntajeWrap::Reset),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("PuntajeNativo", func);
    return exports;
}

// JS: new PuntajeNativo() -- Puntaje() no toma argumentos.
PuntajeWrap::PuntajeWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<PuntajeWrap>(info), puntaje_() {}

Napi::Value PuntajeWrap::GetNivel(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), puntaje_.getNivel());
}

Napi::Value PuntajeWrap::GetPuntos(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), puntaje_.getPuntos());
}

// JS: puntajeNativo.sumar(n)
Napi::Value PuntajeWrap::Sumar(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "sumar(n) espera un numero").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    int n = info[0].As<Napi::Number>().Int32Value();
    puntaje_.sumar(n);
    return env.Undefined();
}

// JS: puntajeNativo.restarVida()
Napi::Value PuntajeWrap::RestarVida(const Napi::CallbackInfo& info) {
    puntaje_.restarVida();
    return info.Env().Undefined();
}

// JS: puntajeNativo.reset()
Napi::Value PuntajeWrap::Reset(const Napi::CallbackInfo& info) {
    puntaje_.reset();
    return info.Env().Undefined();
}
