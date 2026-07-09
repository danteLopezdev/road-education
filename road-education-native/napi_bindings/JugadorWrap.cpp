#include "JugadorWrap.h"

using namespace RoadEducation;

Napi::FunctionReference JugadorWrap::constructor;

Napi::Object JugadorWrap::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "JugadorNativo", {
        InstanceAccessor("x", &JugadorWrap::GetX, nullptr),
        InstanceAccessor("y", &JugadorWrap::GetY, nullptr),
        InstanceAccessor("vidas", &JugadorWrap::GetVidas, nullptr),
        InstanceAccessor("estaVivo", &JugadorWrap::GetEstaVivo, nullptr),
        InstanceMethod("mover", &JugadorWrap::Mover),
        InstanceMethod("perderVida", &JugadorWrap::PerderVida),
        InstanceMethod("resetPosicion", &JugadorWrap::ResetPosicion),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("JugadorNativo", func);
    return exports;
}

// JS: new JugadorNativo(xInicial, yInicial)
JugadorWrap::JugadorWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<JugadorWrap>(info),
      jugador_(
          info.Length() > 0 ? info[0].As<Napi::Number>().FloatValue() : 0.0f,
          info.Length() > 1 ? info[1].As<Napi::Number>().FloatValue() : 0.0f
      ) {}

RoadEducation::Jugador& JugadorWrap::Get() {
    return jugador_;
}

Napi::Value JugadorWrap::GetX(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), jugador_.x);
}

Napi::Value JugadorWrap::GetY(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), jugador_.y);
}

Napi::Value JugadorWrap::GetVidas(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), jugador_.vidas);
}

Napi::Value JugadorWrap::GetEstaVivo(const Napi::CallbackInfo& info) {
    return Napi::Boolean::New(info.Env(), jugador_.estaVivo());
}

// JS: jugadorNativo.mover(dx, dy)
Napi::Value JugadorWrap::Mover(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "mover(dx, dy) espera dos numeros").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    float dx = info[0].As<Napi::Number>().FloatValue();
    float dy = info[1].As<Napi::Number>().FloatValue();
    jugador_.mover(dx, dy);
    return env.Undefined();
}

// JS: jugadorNativo.perderVida()
Napi::Value JugadorWrap::PerderVida(const Napi::CallbackInfo& info) {
    jugador_.perderVida();
    return info.Env().Undefined();
}

// JS: jugadorNativo.resetPosicion()
Napi::Value JugadorWrap::ResetPosicion(const Napi::CallbackInfo& info) {
    jugador_.resetPosicion();
    return info.Env().Undefined();
}
