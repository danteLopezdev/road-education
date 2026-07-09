#ifndef NAPI_OBSTACULO_WRAP_H
#define NAPI_OBSTACULO_WRAP_H

#include <napi.h>
#include "../src/entidades/Obstaculo.h"

// ObstaculoWrap
// Envuelve RoadEducation::Obstaculo como objeto JS ("ObstaculoNativo").
// Responsable: Dante
class ObstaculoWrap : public Napi::ObjectWrap<ObstaculoWrap> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    explicit ObstaculoWrap(const Napi::CallbackInfo& info);

    RoadEducation::Obstaculo& Get();

    // Crea un ObstaculoNativo de JS a partir de un Obstaculo de C++.
    // Usado por EscenarioWrap::ObstaculosActivos para envolver cada
    // elemento del std::vector<Obstaculo> que devuelve el core.
    static Napi::Object NewInstance(Napi::Env env, const RoadEducation::Obstaculo& obstaculo);

private:
    static Napi::FunctionReference constructor;
    RoadEducation::Obstaculo obstaculo_;

    Napi::Value GetX(const Napi::CallbackInfo& info);
    Napi::Value GetY(const Napi::CallbackInfo& info);
    Napi::Value GetAncho(const Napi::CallbackInfo& info);
    Napi::Value GetAlto(const Napi::CallbackInfo& info);
    Napi::Value GetTipo(const Napi::CallbackInfo& info);
    Napi::Value GetActivo(const Napi::CallbackInfo& info);

    // TODO(Dante): implementar
    Napi::Value Activar(const Napi::CallbackInfo& info);
    Napi::Value Desactivar(const Napi::CallbackInfo& info);
    // Recibe un JugadorNativo (ver JugadorWrap) como info[0].
    Napi::Value ColisionaCon(const Napi::CallbackInfo& info);
};

#endif // NAPI_OBSTACULO_WRAP_H
