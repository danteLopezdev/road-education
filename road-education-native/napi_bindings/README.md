# napi_bindings/ — responsable: Dante

> **Nota (2026-07-09):** el compilador de Visual Studio 2026 Insiders tiene
> un bug (Internal Compiler Error / C1001) con la forma "template" de
> `InstanceAccessor<&Clase::Metodo>("nombre")` de node-addon-api. Por eso
> todos los wrappers usan la forma no-template equivalente:
> `InstanceAccessor("nombre", &Clase::Metodo, nullptr)`. Si compilas con
> otro toolchain (VS2022 estable, por ejemplo) ambas formas funcionan igual,
> pero no hay razón para volver a la template — esta es igual de válida y
> más compatible.

Capa que conecta el core en C++ (`src/core/`, de Lutber) y las entidades
(`src/entidades/`, `src/utils/`, de Marlon) con Node/Electron via N-API.

## Estado: completo

Los 4 wrappers (`JugadorWrap`, `ObstaculoWrap`, `EscenarioWrap`,
`PuntajeWrap`) y los 3 grupos de funciones (`FisicaBindings`,
`ColisionesBindings`, `NivelBindings`) están implementados y probados
(`npm test`, y además verificado en runtime, no solo compilado).

Puntos a tener en cuenta si los tocas:

- **`ObstaculoWrap::NewInstance` / `EscenarioWrap::NewInstance`**: helpers
  que envuelven un `Obstaculo`/`Escenario` de C++ (que viene de un
  `std::vector` o de `Nivel::cargarEscenario`) en un objeto JS sin pasar
  por `new` desde JS. `EscenarioWrap::ObstaculosActivos` usa el primero;
  `NivelBindings::CargarEscenario` usa el segundo.
- **Patrón de "unwrap"** usado en todos los métodos que reciben otro
  objeto nativo como argumento (`colisionaCon`, `detectarAABB`,
  `agregarObstaculo`, etc.):
  ```cpp
  if (!info[0].IsObject()) { /* TypeError */ }
  JugadorWrap* jw = Napi::ObjectWrap<JugadorWrap>::Unwrap(info[0].As<Napi::Object>());
  ```
- **Validación de entrada**: cada método valida tipo/cantidad de
  argumentos con `Napi::TypeError` antes de tocar el core. Además,
  `NivelBindings::CargarEscenario` usa `Validador::validarIdNivel` (de
  Marlon) antes de llamar a `Nivel().cargarEscenario()`, y lanza
  `Napi::RangeError` si el id es inválido — tal como estaba pensado en el
  comentario original de `Validador.h`.

## Build

```bash
npm install
npm run build      # node-gyp rebuild
npm test           # test/test_bindings.js
```

Nota: node-gyp en Windows normalmente necesita las Build Tools de Visual
Studio (workload "Desktop development with C++"), no MSYS2/GCC — eso es
solo para compilar `test/pruebas_core.cpp` de forma standalone.
