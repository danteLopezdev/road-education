#include "EscenarioWrap.h"
#include "ObstaculoWrap.h"

using namespace RoadEducation;

Napi::FunctionReference EscenarioWrap::constructor;

Napi::Object EscenarioWrap::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "EscenarioNativo", {
        InstanceAccessor("idNivel", &EscenarioWrap::GetIdNivel, nullptr),
        InstanceAccessor("nombre", &EscenarioWrap::GetNombre, nullptr),
        InstanceMethod("agregarObstaculo", &EscenarioWrap::AgregarObstaculo),
        InstanceMethod("obstaculosActivos", &EscenarioWrap::ObstaculosActivos),
        InstanceMethod("limpiar", &EscenarioWrap::Limpiar),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("EscenarioNativo", func);
    return exports;
}

// JS: new EscenarioNativo(idNivel, nombre)
EscenarioWrap::EscenarioWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<EscenarioWrap>(info),
      escenario_(
          info.Length() > 0 ? info[0].As<Napi::Number>().Int32Value() : 0,
          info.Length() > 1 ? info[1].As<Napi::String>().Utf8Value() : ""
      ) {}

RoadEducation::Escenario& EscenarioWrap::Get() {
    return escenario_;
}

// Usado por NivelBindings::CargarEscenario: envuelve un Escenario de C++
// (incluido su vector<Obstaculo>) en un EscenarioNativo sin pasar por
// "new" en JS.
Napi::Object EscenarioWrap::NewInstance(Napi::Env env, const RoadEducation::Escenario& escenario) {
    Napi::Object obj = constructor.New({
        Napi::Number::New(env, escenario.idNivel),
        Napi::String::New(env, escenario.nombre),
    });
    EscenarioWrap* wrap = Napi::ObjectWrap<EscenarioWrap>::Unwrap(obj);
    wrap->escenario_ = escenario; // copia completa, incluye obstaculos
    return obj;
}

Napi::Value EscenarioWrap::GetIdNivel(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), escenario_.idNivel);
}

Napi::Value EscenarioWrap::GetNombre(const Napi::CallbackInfo& info) {
    return Napi::String::New(info.Env(), escenario_.nombre);
}

// JS: escenarioNativo.agregarObstaculo(obstaculoNativo)
Napi::Value EscenarioWrap::AgregarObstaculo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsObject()) {
        Napi::TypeError::New(env, "agregarObstaculo(obstaculoNativo) espera un ObstaculoNativo").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    ObstaculoWrap* ow = Napi::ObjectWrap<ObstaculoWrap>::Unwrap(info[0].As<Napi::Object>());
    escenario_.agregarObstaculo(ow->Get());
    return env.Undefined();
}

// JS: escenarioNativo.obstaculosActivos() -> ObstaculoNativo[]
Napi::Value EscenarioWrap::ObstaculosActivos(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    std::vector<Obstaculo> activos = escenario_.obstaculosActivos();

    Napi::Array arr = Napi::Array::New(env, activos.size());
    for (size_t i = 0; i < activos.size(); i++) {
        arr[i] = ObstaculoWrap::NewInstance(env, activos[i]);
    }
    return arr;
}

// JS: escenarioNativo.limpiar()
Napi::Value EscenarioWrap::Limpiar(const Napi::CallbackInfo& info) {
    escenario_.limpiar();
    return info.Env().Undefined();
}
