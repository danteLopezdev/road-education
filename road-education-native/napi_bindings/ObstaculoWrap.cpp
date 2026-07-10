#include "ObstaculoWrap.h"
#include "JugadorWrap.h"

using namespace RoadEducation;

Napi::FunctionReference ObstaculoWrap::constructor;

Napi::Object ObstaculoWrap::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "ObstaculoNativo", {
        InstanceAccessor("x", &ObstaculoWrap::GetX, nullptr),
        InstanceAccessor("y", &ObstaculoWrap::GetY, nullptr),
        InstanceAccessor("ancho", &ObstaculoWrap::GetAncho, nullptr),
        InstanceAccessor("alto", &ObstaculoWrap::GetAlto, nullptr),
        InstanceAccessor("tipo", &ObstaculoWrap::GetTipo, nullptr),
        InstanceAccessor("activo", &ObstaculoWrap::GetActivo, nullptr),
        InstanceMethod("activar", &ObstaculoWrap::Activar),
        InstanceMethod("desactivar", &ObstaculoWrap::Desactivar),
        InstanceMethod("colisionaCon", &ObstaculoWrap::ColisionaCon),
        InstanceMethod("mover", &ObstaculoWrap::Mover),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("ObstaculoNativo", func);
    return exports;
}

// JS: new ObstaculoNativo(x, y, ancho, alto, tipo)
ObstaculoWrap::ObstaculoWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<ObstaculoWrap>(info),
      obstaculo_(
          info.Length() > 0 ? info[0].As<Napi::Number>().FloatValue() : 0.0f,
          info.Length() > 1 ? info[1].As<Napi::Number>().FloatValue() : 0.0f,
          info.Length() > 2 ? info[2].As<Napi::Number>().FloatValue() : 0.0f,
          info.Length() > 3 ? info[3].As<Napi::Number>().FloatValue() : 0.0f,
          info.Length() > 4 ? info[4].As<Napi::String>().Utf8Value() : "generico"
      ) {}

RoadEducation::Obstaculo& ObstaculoWrap::Get() {
    return obstaculo_;
}

// Usado por EscenarioWrap::ObstaculosActivos: envuelve un Obstaculo de C++
// (que viene de un std::vector, sin pasar por "new" en JS) en un objeto
// ObstaculoNativo utilizable desde JS.
Napi::Object ObstaculoWrap::NewInstance(Napi::Env env, const RoadEducation::Obstaculo& obstaculo) {
    Napi::Object obj = constructor.New({
        Napi::Number::New(env, obstaculo.x),
        Napi::Number::New(env, obstaculo.y),
        Napi::Number::New(env, obstaculo.ancho),
        Napi::Number::New(env, obstaculo.alto),
        Napi::String::New(env, obstaculo.tipo),
    });
    ObstaculoWrap* wrap = Napi::ObjectWrap<ObstaculoWrap>::Unwrap(obj);
    wrap->obstaculo_.activo = obstaculo.activo; // el constructor siempre deja activo=true; igualamos el estado real
    return obj;
}

Napi::Value ObstaculoWrap::GetX(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), obstaculo_.x);
}

Napi::Value ObstaculoWrap::GetY(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), obstaculo_.y);
}

Napi::Value ObstaculoWrap::GetAncho(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), obstaculo_.ancho);
}

Napi::Value ObstaculoWrap::GetAlto(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), obstaculo_.alto);
}

Napi::Value ObstaculoWrap::GetTipo(const Napi::CallbackInfo& info) {
    return Napi::String::New(info.Env(), obstaculo_.tipo);
}

Napi::Value ObstaculoWrap::GetActivo(const Napi::CallbackInfo& info) {
    return Napi::Boolean::New(info.Env(), obstaculo_.activo);
}

// JS: obstaculoNativo.activar()
Napi::Value ObstaculoWrap::Activar(const Napi::CallbackInfo& info) {
    obstaculo_.activar();
    return info.Env().Undefined();
}

// JS: obstaculoNativo.desactivar()
Napi::Value ObstaculoWrap::Desactivar(const Napi::CallbackInfo& info) {
    obstaculo_.desactivar();
    return info.Env().Undefined();
}

// JS: obstaculoNativo.colisionaCon(jugadorNativo)
Napi::Value ObstaculoWrap::ColisionaCon(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsObject()) {
        Napi::TypeError::New(env, "colisionaCon(jugadorNativo) espera un JugadorNativo").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    JugadorWrap* jw = Napi::ObjectWrap<JugadorWrap>::Unwrap(info[0].As<Napi::Object>());
    bool resultado = obstaculo_.colisionaCon(jw->Get());
    return Napi::Boolean::New(env, resultado);
}

// JS: obstaculoNativo.mover(dx, dy)
Napi::Value ObstaculoWrap::Mover(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "mover(dx, dy) espera dos numeros").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    float dx = info[0].As<Napi::Number>().FloatValue();
    float dy = info[1].As<Napi::Number>().FloatValue();
    obstaculo_.mover(dx, dy);
    return env.Undefined();
}
