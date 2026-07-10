#ifndef NAPI_JUGADOR_WRAP_H
#define NAPI_JUGADOR_WRAP_H

#include <napi.h>
#include "../src/entidades/Jugador.h"

// JugadorWrap
// Envuelve RoadEducation::Jugador como objeto JS ("JugadorNativo") via
// Napi::ObjectWrap, para que main.js pueda hacer:
//   const j = new native.JugadorNativo(xInicial, yInicial);
//   j.mover(dx, dy);
//
// Responsable: Dante
//
// Estado del esqueleto:
// - Init() y el constructor ya registran la clase y funcionan.
// - Los getters (x, y, vidas, estaVivo) ya funcionan.
// - Los metodos (mover, perderVida, resetPosicion) estan pendientes:
//   lanzan "no implementado" hasta completarlos.
class JugadorWrap : public Napi::ObjectWrap<JugadorWrap> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    explicit JugadorWrap(const Napi::CallbackInfo& info);

    // Acceso directo al objeto C++ envuelto, para que otros bindings
    // (FisicaBindings, ColisionesBindings) puedan operar sobre el mismo
    // Jugador sin pasar por JS. Ver FisicaBindings::CalcularPosicion.
    RoadEducation::Jugador& Get();

private:
    static Napi::FunctionReference constructor;
    RoadEducation::Jugador jugador_;

    // --- Propiedades de solo lectura (accessors) ---
    Napi::Value GetX(const Napi::CallbackInfo& info);
    Napi::Value GetY(const Napi::CallbackInfo& info);
    Napi::Value GetVidas(const Napi::CallbackInfo& info);
    Napi::Value GetEstaVivo(const Napi::CallbackInfo& info);

    // --- Metodos (TODO: implementar el cuerpo real) ---
    Napi::Value Mover(const Napi::CallbackInfo& info);
    Napi::Value PerderVida(const Napi::CallbackInfo& info);
    Napi::Value ResetPosicion(const Napi::CallbackInfo& info);
    Napi::Value ResetCompleto(const Napi::CallbackInfo& info);
};

#endif // NAPI_JUGADOR_WRAP_H
