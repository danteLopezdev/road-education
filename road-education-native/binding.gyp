{
  "targets": [
    {
      "target_name": "road_education_native",
      "sources": [
        "napi_bindings/addon.cpp",
        "napi_bindings/JugadorWrap.cpp",
        "napi_bindings/ObstaculoWrap.cpp",
        "napi_bindings/EscenarioWrap.cpp",
        "napi_bindings/PuntajeWrap.cpp",
        "napi_bindings/FisicaBindings.cpp",
        "napi_bindings/ColisionesBindings.cpp",
        "napi_bindings/NivelBindings.cpp",
        "src/core/Fisica.cpp",
        "src/core/Colisiones.cpp",
        "src/core/Puntaje.cpp",
        "src/core/Nivel.cpp",
        "src/entidades/Jugador.cpp",
        "src/entidades/Obstaculo.cpp",
        "src/entidades/Escenario.cpp",
        "src/utils/Validador.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "src"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
      "cflags_cc": [ "-std=c++17" ],
      "xcode_settings": {
        "CLANG_CXX_LANGUAGE_STANDARD": "c++17",
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
      },
      "msvs_settings": {
        "VCCLCompilerTool": {
          "ExceptionHandling": 1,
          "AdditionalOptions": [ "/std:c++17" ]
        }
      }
    }
  ]
}
