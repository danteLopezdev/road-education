#ifndef NAPI_ESCENARIO_WRAP_H
#define NAPI_ESCENARIO_WRAP_H

#include <napi.h>
#include "../src/entidades/Escenario.h"

// EscenarioWrap
// Envuelve RoadEducation::Escenario como objeto JS ("EscenarioNativo").
// Responsable: Dante
//
// Este es el wrapper mas delicado del esqueleto: Escenario contiene un
// std::vector<Obstaculo>, y obstaculosActivos() debe devolver un array JS
// de ObstaculoNativo (cada uno envuelto de nuevo con ObstaculoWrap). Por
// eso NewInstance() esta declarado como helper publico: lo va a necesitar
// NivelBindings::CargarEscenario para construir un EscenarioNativo desde
// C++ sin pasar por "new" en JS.
class EscenarioWrap : public Napi::ObjectWrap<EscenarioWrap> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    explicit EscenarioWrap(const Napi::CallbackInfo& info);

    RoadEducation::Escenario& Get();

    // Crea un EscenarioNativo de JS a partir de un Escenario de C++.
    // TODO(Dante): implementar usando constructor.New({...}) y luego
    // copiar/mover el contenido a la instancia envuelta.
    static Napi::Object NewInstance(Napi::Env env, const RoadEducation::Escenario& escenario);

private:
    static Napi::FunctionReference constructor;
    RoadEducation::Escenario escenario_;

    Napi::Value GetIdNivel(const Napi::CallbackInfo& info);
    Napi::Value GetNombre(const Napi::CallbackInfo& info);

    // TODO(Dante): implementar
    Napi::Value AgregarObstaculo(const Napi::CallbackInfo& info);
    Napi::Value ObstaculosActivos(const Napi::CallbackInfo& info);
    Napi::Value Limpiar(const Napi::CallbackInfo& info);

    // JS: escenarioNativo.moverObstaculo(index, dx, dy)
    // A diferencia de obstaculosActivos() (que devuelve COPIAS), este
    // metodo muta el Obstaculo real dentro de escenario_.obstaculos,
    // por indice. Necesario porque obstaculosActivos() no sirve para
    // mover autos de forma persistente (ver discusion en el chat).
    Napi::Value MoverObstaculo(const Napi::CallbackInfo& info);
};

#endif // NAPI_ESCENARIO_WRAP_H
