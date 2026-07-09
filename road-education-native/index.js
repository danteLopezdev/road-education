// index.js
// Punto de entrada JS del addon. Lo usa main.js (proceso principal de
// Electron) via require('road-education-native'), nunca el renderer
// directamente (regla de la arquitectura: acceso nativo solo desde main).
//
// 'bindings' localiza el .node compilado sin importar Debug/Release
// ni el layout exacto de build/.
module.exports = require('bindings')('road_education_native');
const { app, BrowserWindow } = require('electron');
const path = require('path');

function crearVentana() {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      contextIsolation: true,
      nodeIntegration: false
    }
  });

  win.loadFile(path.join(__dirname, '..', 'index.html'));
}

app.whenReady().then(crearVentana);
