// main.js
// Proceso principal de Electron para Road Education.
// Ubicado en road-education-native/ (mismo nivel que package.json).
//
// Cadena de acceso (ver README del proyecto):
//   renderer (Canvas 2D) -> ipcRenderer -> preload.js (contextBridge)
//   -> main.js (este archivo) -> require('./index.js') -> addon nativo C++
//
// Responsable de esta capa: Dante

const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const fs = require('fs');
const addon = require('./index.js'); // delega en 'bindings' -> road_education_native.node

let mainWindow;

// ---------- Instancias del addon nativo ----------
// Se mantienen vivas durante toda la partida; los handlers IPC operan
// sobre estas mismas instancias en cada llamada (no se recrean por frame).

const CELL = 40; // debe coincidir con CELL en game.js
const jugadorInstance = new addon.JugadorNativo(5 * CELL, 14 * CELL); // celda {5,14} exacta, ver inicioJugador de nivel1.json
const puntajeInstance = new addon.PuntajeNativo();

// El escenario se construye desde el JSON del nivel (ver nivel:cargarDesdeJson).
// Arranca vacio hasta que game.js pida cargar el nivel 1.
let escenarioInstance = new addon.EscenarioNativo(0, 'vacio');

function crearVentana() {
  mainWindow = new BrowserWindow({
    width: 800,
    height: 700,
    minWidth: 500,
    minHeight: 500,
    resizable: true,
    autoHideMenuBar: true,
    webPreferences: {
      contextIsolation: true,
      nodeIntegration: false,
      preload: path.join(__dirname, 'preload.js'),
    },
  });

  // main.js vive en road-education-native/, las vistas un nivel arriba.
  mainWindow.loadFile(path.join(__dirname, '..', 'index.html'));
}

app.whenReady().then(() => {
  crearVentana();

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) crearVentana();
  });
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') app.quit();
});

// ---------- Handlers IPC ----------

// --- Fisica ---
// Nota: en el diseño de niveles por grid (ver doc de diseño), el jugador
// se mueve por celdas discretas via jugador:mover, no por fisica continua.
// Estos tres quedan disponibles por si se necesitan (ej. animaciones de
// autos con aceleracion), pero no son parte del flujo principal del grid.

ipcMain.handle('fisica:calcularPosicion', (event, delta) => {
  addon.calcularPosicion(jugadorInstance, delta);
  return { x: jugadorInstance.x, y: jugadorInstance.y };
});

ipcMain.handle('fisica:aplicarGravedad', () => {
  addon.aplicarGravedad(jugadorInstance);
  return { x: jugadorInstance.x, y: jugadorInstance.y };
});

ipcMain.handle('fisica:calcularVelocidad', (event, vel, acel, delta) => {
  return addon.calcularVelocidad(vel, acel, delta);
});

// --- Jugador ---

ipcMain.handle('jugador:mover', (event, dx, dy) => {
  jugadorInstance.mover(dx, dy);
  return { x: jugadorInstance.x, y: jugadorInstance.y };
});

ipcMain.handle('jugador:perderVida', () => {
  jugadorInstance.perderVida();
  return { vidas: jugadorInstance.vidas, estaVivo: jugadorInstance.estaVivo };
});

ipcMain.handle('jugador:resetPosicion', () => {
  jugadorInstance.resetPosicion();
  return { x: jugadorInstance.x, y: jugadorInstance.y };
});

ipcMain.handle('jugador:resetCompleto', () => {
  jugadorInstance.resetCompleto();
  return {
    x: jugadorInstance.x,
    y: jugadorInstance.y,
    vidas: jugadorInstance.vidas,
    estaVivo: jugadorInstance.estaVivo,
  };
});

ipcMain.handle('jugador:estado', () => {
  return {
    x: jugadorInstance.x,
    y: jugadorInstance.y,
    vidas: jugadorInstance.vidas,
    estaVivo: jugadorInstance.estaVivo,
  };
});

// --- Colisiones ---
// detectarColision: recorre solo los obstaculos activos del escenario
// actual. La optimizacion de "filtrar solo autos de la fila del jugador"
// (ver doc de diseño, seccion 6) se hace del lado de game.js antes de
// pedir a main.js que revise colision, para no acoplar el addon al
// concepto de "fila" (que es puramente del diseño de grid, no del core).

ipcMain.handle('colisiones:detectarColision', () => {
  const obstaculos = escenarioInstance.obstaculosActivos();
  return obstaculos.some((obstaculo) => addon.detectarAABB(jugadorInstance, obstaculo));
});

ipcMain.handle('colisiones:estaEnZona', (event, x, y) => {
  return addon.estaEnZona(x, y, escenarioInstance);
});

// --- Nivel / Escenario ---
// El escenario se construye ENTERAMENTE desde el JSON que lee game.js
// (fuente unica de verdad para el diseño de niveles). No se usa
// addon.cargarEscenario(id) para poblar datos -- ver discusion de
// arquitectura en el chat con Claude / registro de modificaciones.

// Lee un archivo JSON de niveles/ desde disco. Se hace aqui (proceso
// principal, con acceso a fs) en vez de fetch() en el renderer, porque
// fetch() de archivos locales bajo file:// puede fallar por CORS segun
// la configuracion de seguridad de Electron.
ipcMain.handle('nivel:leerArchivo', (event, nombreArchivo) => {
  const rutaArchivo = path.join(__dirname, '..', 'niveles', nombreArchivo);

  if (!fs.existsSync(rutaArchivo)) {
    throw new Error(`No existe el archivo de nivel: ${rutaArchivo}`);
  }

  const contenido = fs.readFileSync(rutaArchivo, 'utf-8');

  if (!contenido.trim()) {
    throw new Error(
      `"${nombreArchivo}" está vacío (0 bytes) en ${rutaArchivo}. ` +
      `Verifica que el contenido del nivel se haya guardado ahí.`
    );
  }

  try {
    return JSON.parse(contenido);
  } catch (error) {
    throw new Error(`"${nombreArchivo}" tiene JSON inválido: ${error.message}`);
  }
});

ipcMain.handle('nivel:cargarDesdeJson', (event, nivelData) => {
  escenarioInstance = new addon.EscenarioNativo(nivelData.idNivel, nivelData.nombre);

  // Autos de cada carril -> Obstaculo tipo 'auto'
  (nivelData.carriles || []).forEach((carril) => {
    (carril.autos || []).forEach((auto) => {
      const obstaculo = new addon.ObstaculoNativo(
        auto.colInicial * 40,
        carril.fila * 40,
        40,
        40,
        'auto'
      );
      escenarioInstance.agregarObstaculo(obstaculo);
    });
  });

  // Reset de jugador y puntaje al iniciar/reiniciar un nivel
  jugadorInstance.resetCompleto();
  puntajeInstance.reset();

  return {
    idNivel: escenarioInstance.idNivel,
    nombre: escenarioInstance.nombre,
  };
});

ipcMain.handle('nivel:validarRegla', (event, accion) => {
  return addon.validarRegla(accion);
});

ipcMain.handle('nivel:esCompletado', () => {
  return addon.esCompletado(escenarioInstance);
});

ipcMain.handle('escenario:obstaculosActivos', () => {
  return escenarioInstance.obstaculosActivos().map((o) => ({
    x: o.x,
    y: o.y,
    ancho: o.ancho,
    alto: o.alto,
    tipo: o.tipo,
    activo: o.activo,
  }));
});

// moverObstaculos: mueve una lista de obstaculos por indice (el indice
// coincide con el orden de obstaculosActivos()). Usa
// escenarioInstance.moverObstaculo(), que muta el obstaculo real dentro
// del escenario -- NO usa obstaculosActivos()+mover(), porque esa
// combinacion opera sobre copias y el movimiento no persistiria entre
// frames (ver discusion de arquitectura).
ipcMain.handle('escenario:moverObstaculos', (event, movimientos) => {
  movimientos.forEach(({ index, dx, dy }) => {
    escenarioInstance.moverObstaculo(index, dx, dy ?? 0);
  });
});

// --- Puntaje ---

ipcMain.handle('puntaje:sumarPuntos', (event, n) => {
  puntajeInstance.sumar(n);
  return { puntos: puntajeInstance.puntos, nivel: puntajeInstance.nivel };
});

ipcMain.handle('puntaje:getNivel', () => {
  return puntajeInstance.nivel;
});

ipcMain.handle('puntaje:restarVida', () => {
  puntajeInstance.restarVida();
  return { puntos: puntajeInstance.puntos, nivel: puntajeInstance.nivel };
});

ipcMain.handle('puntaje:reset', () => {
  puntajeInstance.reset();
  return { puntos: puntajeInstance.puntos, nivel: puntajeInstance.nivel };
});

// --- Navegacion ---

ipcMain.handle('navegacion:irAJuego', () => {
  mainWindow.loadFile(path.join(__dirname, '..', 'game.html'));
});

ipcMain.handle('navegacion:irAMenu', () => {
  mainWindow.loadFile(path.join(__dirname, '..', 'index.html'));
});