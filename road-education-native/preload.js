// preload.js
// Puente seguro entre el renderer (Canvas 2D) y main.js, via
// contextBridge + ipcRenderer.invoke. contextIsolation: true y
// nodeIntegration: false en el BrowserWindow (ver main.js) obligan a
// pasar por aqui -- el renderer nunca toca Node ni el addon directo.
//
// Cadena de acceso (ver README del proyecto):
//   renderer (Canvas 2D) -> ipcRenderer -> preload.js (este archivo)
//   -> main.js -> require('./index.js') -> addon nativo C++
//
// Responsable de esta capa: Dante

const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('api', {
  // --- Fisica ---
  // Disponible por si se necesita (ej. animaciones con aceleracion);
  // el movimiento del jugador por grid usa jugador.mover, no esto.
  calcularPosicion: (delta) => ipcRenderer.invoke('fisica:calcularPosicion', delta),
  aplicarGravedad: () => ipcRenderer.invoke('fisica:aplicarGravedad'),
  calcularVelocidad: (vel, acel, delta) =>
    ipcRenderer.invoke('fisica:calcularVelocidad', vel, acel, delta),

  // --- Jugador ---
  moverJugador: (dx, dy) => ipcRenderer.invoke('jugador:mover', dx, dy),
  perderVida: () => ipcRenderer.invoke('jugador:perderVida'),
  resetPosicionJugador: () => ipcRenderer.invoke('jugador:resetPosicion'),
  resetCompletoJugador: () => ipcRenderer.invoke('jugador:resetCompleto'),
  getEstadoJugador: () => ipcRenderer.invoke('jugador:estado'),

  // --- Colisiones ---
  detectarColision: () => ipcRenderer.invoke('colisiones:detectarColision'),
  estaEnZona: (x, y) => ipcRenderer.invoke('colisiones:estaEnZona', x, y),

  // --- Nivel / Escenario ---
  cargarNivelDesdeJson: (nivelData) => ipcRenderer.invoke('nivel:cargarDesdeJson', nivelData),
  validarRegla: (accion) => ipcRenderer.invoke('nivel:validarRegla', accion),
  esCompletado: () => ipcRenderer.invoke('nivel:esCompletado'),
  getObstaculosActivos: () => ipcRenderer.invoke('escenario:obstaculosActivos'),
  moverObstaculos: (movimientos) => ipcRenderer.invoke('escenario:moverObstaculos', movimientos),

  // --- Puntaje ---
  sumarPuntos: (n) => ipcRenderer.invoke('puntaje:sumarPuntos', n),
  getNivel: () => ipcRenderer.invoke('puntaje:getNivel'),
  restarVidaPuntaje: () => ipcRenderer.invoke('puntaje:restarVida'),
  resetPuntaje: () => ipcRenderer.invoke('puntaje:reset'),

  // --- Navegacion ---
  irAJuego: () => ipcRenderer.invoke('navegacion:irAJuego'),
  irAMenu: () => ipcRenderer.invoke('navegacion:irAMenu'),
});