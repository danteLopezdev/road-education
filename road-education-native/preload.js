// preload.js
// Puente seguro entre el renderer (game.js) y el proceso principal (main.js).
// Aquí NO se importa el addon C++ directamente: eso vive en main.js.
// Este archivo solo expone funciones "seguras" al mundo del renderer.

const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('api', {
  // ---- FÍSICA ----
  // Recibe posición, velocidad y delta time; devuelve la nueva posición calculada por el addon C++.
  calcularMovimiento: (x, y, vel, delta) =>
    ipcRenderer.invoke('fisica:calcularMovimiento', { x, y, vel, delta }),

  // ---- COLISIONES ----
  // Recibe objetos jugador y obstaculo (planos, serializables); devuelve boolean.
  detectarColision: (jugador, obstaculo) =>
    ipcRenderer.invoke('colisiones:detectarColision', { jugador, obstaculo }),

  // ---- PUNTAJE ----
  sumarPuntos: (n) =>
    ipcRenderer.invoke('puntaje:sumarPuntos', { n }),

  getNivel: () =>
    ipcRenderer.invoke('puntaje:getNivel'),

  restarVida: () =>
    ipcRenderer.invoke('puntaje:restarVida'),

  resetPuntaje: () =>
    ipcRenderer.invoke('puntaje:reset'),

  // ---- NAVEGACIÓN ----
  irAJuego: () =>
    ipcRenderer.invoke('navegacion:irAJuego'),

  irAMenu: () =>
    ipcRenderer.invoke('navegacion:irAMenu'),
});
