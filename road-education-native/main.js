// main.js
// Proceso principal de Electron. Aquí y SOLO aquí se carga el addon nativo C++.

const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');

// 1. Cargar el addon nativo compilado.
//    main.js vive junto a index.js (mismo folder que package.json),
//    e index.js ya expone el addon via require('bindings').
const addon = require('./index.js');

let mainWindow;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 900,
    height: 700,
    resizable: false,       // look de app de juego, no de ventana web redimensionable
    autoHideMenuBar: true,  // oculta la barra de menú por defecto de Electron (File, Edit, View...)
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      contextIsolation: true, // obligatorio: permite que contextBridge funcione de forma segura
      nodeIntegration: false, // el renderer no debe tener acceso directo a Node/require
    },
  });

  mainWindow.loadFile(path.join(__dirname, '..', 'index.html')); // arranca en el menú
  mainWindow.webContents.openDevTools();

  // Descomenta esto mientras desarrollas, para ver la consola del renderer:
  // mainWindow.webContents.openDevTools();
}

app.whenReady().then(() => {
  createWindow();

  app.on('activate', () => {
    // En macOS es común recrear la ventana si no hay ninguna abierta
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
  });
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') app.quit();
});

// ---------------------------------------------------------------------------
// 2. Handlers IPC: reciben la petición del renderer (via preload.js),
//    llaman al addon C++ y devuelven el resultado.
// ---------------------------------------------------------------------------

ipcMain.handle('fisica:calcularMovimiento', (event, { x, y, vel, delta }) => {
  return addon.calcularMovimiento(x, y, vel, delta);
});

ipcMain.handle('colisiones:detectarColision', (event, { jugador, obstaculo }) => {
  return addon.detectarColision(jugador, obstaculo);
});

ipcMain.handle('puntaje:sumarPuntos', (event, { n }) => {
  return addon.sumarPuntos(n);
});

ipcMain.handle('puntaje:getNivel', () => {
  return addon.getNivel();
});

// NOTA: estas dos no aparecen todavía en la tabla de bindings del doc (§4.4).
// Coméntalas o pídele a Dante que agregue los bindings en C++ si aún no existen.
// ipcMain.handle('puntaje:restarVida', () => {
//   return addon.restarVida();
// });

// ipcMain.handle('puntaje:reset', () => {
//   return addon.reset();
// });

// ---------------------------------------------------------------------------
// 3. Navegación entre pantallas (menú -> juego)
//    Se invoca desde el renderer, ej: window.api.irAJuego()
// ---------------------------------------------------------------------------

ipcMain.handle('navegacion:irAJuego', () => {
  mainWindow.loadFile(path.join(__dirname, '..', 'game.html'));
});

ipcMain.handle('navegacion:irAMenu', () => {
  mainWindow.loadFile(path.join(__dirname, '..', 'index.html'));
});
