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

// ---- Estado básico del juego (ejemplo mínimo, ajusta a tu lógica real) ----
let jugador = { x: 180, y: 500, velX: 0, velY: 0, vidas: 3 };
let ultimoTimestamp = null;

async function actualizarPuntaje(puntos) {
  const nuevoPuntaje = await window.api.sumarPuntos(puntos);
  puntajeValorEl.textContent = nuevoPuntaje;
}

function dibujar() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  // Jugador (placeholder simple, reemplaza con tu sprite/dibujo real)
  ctx.fillStyle = '#00c8ff';
  ctx.fillRect(jugador.x, jugador.y, 40, 60);
}

async function loop(timestamp) {
  if (ultimoTimestamp === null) ultimoTimestamp = timestamp;
  const delta = (timestamp - ultimoTimestamp) / 1000;
  ultimoTimestamp = timestamp;

  // Ejemplo: pedirle al addon la nueva posición del jugador
  // const nuevaPos = await window.api.calcularMovimiento(jugador.x, jugador.y, jugador.velX, delta);
  // jugador.x = nuevaPos.x;
  // jugador.y = nuevaPos.y;

  dibujar();
  requestAnimationFrame(loop);
}

requestAnimationFrame(loop);