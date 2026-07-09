#include "NivelBindings.h"
#include "../src/core/Nivel.h"
#include "../src/utils/Validador.h"
#include "EscenarioWrap.h"

using namespace RoadEducation;

Napi::Object NivelBindings::Init(Napi::Env env, Napi::Object exports) {
    exports.Set("validarRegla", Napi::Function::New(env, ValidarRegla));
    exports.Set("cargarEscenario", Napi::Function::New(env, CargarEscenario));
    exports.Set("esCompletado", Napi::Function::New(env, EsCompletado));
    return exports;
}

// JS: validarRegla(accion: string) -> boolean
Napi::Value NivelBindings::ValidarRegla(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "validarRegla(accion) espera un string").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::string accion = info[0].As<Napi::String>().Utf8Value();
    bool resultado = Nivel().validarRegla(accion);
    return Napi::Boolean::New(env, resultado);
}

// JS: cargarEscenario(id: number) -> EscenarioNativo
Napi::Value NivelBindings::CargarEscenario(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "cargarEscenario(id) espera un numero").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    int id = info[0].As<Napi::Number>().Int32Value();

    // Los datos que llegan desde JS se validan antes de tocar el core
    // (ver el comentario de intencion en utils/Validador.h).
    if (!Validador::validarIdNivel(id)) {
        Napi::RangeError::New(env, "id de nivel invalido").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    Escenario esc = Nivel().cargarEscenario(id);
    return EscenarioWrap::NewInstance(env, esc);
}

// JS: esCompletado(escenarioNativo) -> boolean
Napi::Value NivelBindings::EsCompletado(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsObject()) {
        Napi::TypeError::New(env, "esCompletado(escenarioNativo) espera un EscenarioNativo").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    EscenarioWrap* ew = Napi::ObjectWrap<EscenarioWrap>::Unwrap(info[0].As<Napi::Object>());
    bool resultado = Nivel().esCompletado(ew->Get());
    return Napi::Boolean::New(env, resultado);
}
