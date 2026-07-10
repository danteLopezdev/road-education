// scripts/game.js
// Sin Firebase ni login: el puntaje vive solo en memoria durante la partida,
// calculado por el addon nativo C++ (via window.api, expuesto en preload.js).

const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');
const puntajeValorEl = document.getElementById('puntajeValor');

// ---- Botón Salir: vuelve al menú principal ----
document.getElementById('btnSalir').addEventListener('click', () => {
  window.api.irAMenu();
});

// ---- Configuración del grid (ver doc de diseño de niveles, sección 2) ----
const CELL = 40; // px por celda
const COLS = 10; // 0..9
const ROWS = 15; // 0..14
const COOLDOWN_MS = 150;

// Celda inicial del jugador. TODO: reemplazar por inicioJugador del JSON
// del nivel cuando se conecte nivel:cargarDesdeJson.
let celda = { col: 5, fila: 14 };
let ultimoMovimiento = 0; // timestamp del último movimiento aceptado

// Mapea cada tecla a un delta de celda {dCol, dFila}
const TECLAS = {
  ArrowUp: { dCol: 0, dFila: -1 },
  ArrowDown: { dCol: 0, dFila: 1 },
  ArrowLeft: { dCol: -1, dFila: 0 },
  ArrowRight: { dCol: 1, dFila: 0 },
};

async function manejarTecla(event) {
  const delta = TECLAS[event.key];
  if (!delta) return; // tecla irrelevante, se ignora

  const ahora = performance.now();
  if (ahora - ultimoMovimiento < COOLDOWN_MS) return; // cooldown activo

  const nuevaCol = celda.col + delta.dCol;
  const nuevaFila = celda.fila + delta.dFila;

  // Límites de la cuadrícula: movimiento inválido se ignora silenciosamente
  // (ver doc de diseño, sección 3)
  if (nuevaCol < 0 || nuevaCol >= COLS || nuevaFila < 0 || nuevaFila >= ROWS) {
    return;
  }

  ultimoMovimiento = ahora;
  celda.col = nuevaCol;
  celda.fila = nuevaFila;

  // Sincroniza al jugador del addon (en píxeles) para que las colisiones
  // AABB futuras (Colisiones::detectarAABB) usen la posición real.
  const dxPx = delta.dCol * CELL;
  const dyPx = delta.dFila * CELL;
  await window.api.moverJugador(dxPx, dyPx);

  dibujar();
}

window.addEventListener('keydown', manejarTecla);

// ---- Puntaje ----

function pintarPuntaje(estadoPuntaje) {
  puntajeValorEl.textContent = estadoPuntaje.puntos;
}

async function actualizarPuntaje(puntos) {
  const estado = await window.api.sumarPuntos(puntos);
  pintarPuntaje(estado);
}

async function inicializarHud() {
  const estado = await window.api.resetPuntaje(); // arranque limpio de partida
  pintarPuntaje(estado);
}

// ---- Dibujo ----
// El jugador se dibuja como un cuadrado de una celda completa (CELL x CELL)
// para que se vea alineado al grid. Nota: el addon usa una hitbox fija de
// 32x32 para colisiones (ver Colisiones::detectarAABB) independientemente
// de este tamaño visual -- no afecta la lógica, solo el dibujo.

function dibujar() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  ctx.fillStyle = '#00c8ff';
  ctx.fillRect(celda.col * CELL, celda.fila * CELL, CELL, CELL);
}

// ---- Loop principal ----
// El movimiento del jugador es discreto (por tecla, ver manejarTecla),
// no depende de este loop. El loop queda listo para animación de autos
// en el siguiente paso (carriles + semáforos).

let ultimoTimestamp = null;

function loop(timestamp) {
  if (ultimoTimestamp === null) ultimoTimestamp = timestamp;
  const delta = (timestamp - ultimoTimestamp) / 1000;
  ultimoTimestamp = timestamp;

  // Aquí irá, en el siguiente paso: mover autos vía
  // window.api.moverObstaculos(...) y redibujarlos.

  requestAnimationFrame(loop);
}

dibujar(); // primer frame, antes de cualquier input
inicializarHud();
requestAnimationFrame(loop);