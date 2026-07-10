// index.js
// Punto de entrada JS del addon. Lo usa main.js (proceso principal de
// Electron) via require('road-education-native'), nunca el renderer
// directamente (regla de la arquitectura: acceso nativo solo desde main).
//
// 'bindings' localiza el .node compilado sin importar Debug/Release
// ni el layout exacto de build/.
module.exports = require('bindings')('road_education_native');