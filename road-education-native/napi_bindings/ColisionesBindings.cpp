#include "ColisionesBindings.h"
#include "../src/core/Colisiones.h"
#include "JugadorWrap.h"
#include "ObstaculoWrap.h"
#include "EscenarioWrap.h"

using namespace RoadEducation;

Napi::Object ColisionesBindings::Init(Napi::Env env, Napi::Object exports) {
    exports.Set("detectarAABB", Napi::Function::New(env, DetectarAABB));
    exports.Set("estaEnZona", Napi::Function::New(env, EstaEnZona));
    return exports;
}

// JS: detectarAABB(jugadorNativo, obstaculoNativo) -> boolean
Napi::Value ColisionesBindings::DetectarAABB(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsObject()) {
        Napi::TypeError::New(env, "detectarAABB(jugadorNativo, obstaculoNativo) espera dos objetos").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    JugadorWrap* jw = Napi::ObjectWrap<JugadorWrap>::Unwrap(info[0].As<Napi::Object>());
    ObstaculoWrap* ow = Napi::ObjectWrap<ObstaculoWrap>::Unwrap(info[1].As<Napi::Object>());

    bool resultado = Colisiones().detectarAABB(jw->Get(), ow->Get());
    return Napi::Boolean::New(env, resultado);
}

// JS: estaEnZona(x, y, escenarioNativo) -> boolean
Napi::Value ColisionesBindings::EstaEnZona(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 3 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsObject()) {
        Napi::TypeError::New(env, "estaEnZona(x, y, escenarioNativo) espera (numero, numero, objeto)").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    float x = info[0].As<Napi::Number>().FloatValue();
    float y = info[1].As<Napi::Number>().FloatValue();
    EscenarioWrap* ew = Napi::ObjectWrap<EscenarioWrap>::Unwrap(info[2].As<Napi::Object>());

    bool resultado = Colisiones().estaEnZona(x, y, ew->Get());
    return Napi::Boolean::New(env, resultado);
}
