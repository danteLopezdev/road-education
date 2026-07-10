// scripts/game.js
// Sin Firebase ni login: el puntaje vive solo en memoria durante la partida,
// calculado por el addon nativo C++ (via window.api, expuesto en preload.js).

const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');
const puntajeValorEl = document.getElementById('puntajeValor');
const vidasValorEl = document.getElementById('vidasValor');

// ---- Botón Salir: vuelve al menú principal ----
document.getElementById('btnSalir').addEventListener('click', () => {
  window.api.irAMenu();
});

// ---- Configuración del grid (ver doc de diseño de niveles, sección 2) ----
const CELL = 40; // px por celda -- debe coincidir con CELL en main.js
const COLS = 10; // 0..9
const ROWS = 15; // 0..14
const COOLDOWN_MS = 150;
const INVULNERABILIDAD_MS = 800;

// Secuencia de niveles. Agregar 'nivel2.json', 'nivel3.json' aquí cuando
// existan (ver doc de diseño, sección 9).
const NIVELES = ['nivel1.json'];
let indiceNivelActual = 0;

let celda = { col: 5, fila: 14 };
let metaActual = null; // { col, fila } -- se fija al cargar el nivel
let obstaculosCache = [];
let autosInfo = []; // [{direccion, velocidad, carrilIndex}]
let semaforosInfo = []; // [{col, fila, cicloMs, carrilAsociado}]
let tiempoInicioNivel = 0; // performance.now() al cargar el nivel; base del ciclo de semáforos
let ultimoMovimiento = 0;

let enInvulnerabilidad = false;
let procesandoColision = false;

// ---- Carga de nivel ----

async function cargarNivel(nombreArchivo) {
  const nivelData = await window.api.leerNivelJson(nombreArchivo);

  await window.api.cargarNivelDesdeJson(nivelData);

  celda = { ...nivelData.inicioJugador };
  metaActual = { ...nivelData.meta };
  obstaculosCache = await window.api.getObstaculosActivos();

  autosInfo = [];
  nivelData.carriles.forEach((carril, carrilIndex) => {
    carril.autos.forEach(() => {
      autosInfo.push({ direccion: carril.direccion, velocidad: carril.velocidad, carrilIndex });
    });
  });

  semaforosInfo = (nivelData.semaforos || []).map((s) => ({ ...s }));
  tiempoInicioNivel = performance.now();

  enInvulnerabilidad = false;
  procesandoColision = false;

  const estadoJugador = await window.api.getEstadoJugador();
  pintarVidas(estadoJugador.vidas);

  const estadoPuntaje = await window.api.resetPuntaje();
  pintarPuntaje(estadoPuntaje);

  dibujar();
}

// ---- Semáforos ----
// Alternan entre dos estados con un ciclo fijo en ms, sincronizado con el
// reloj del juego (no con input del jugador). Cuando el semáforo está en
// rojo para autos, el carril asociado se detiene de verdad (no es solo
// decorativo) -- ver doc de diseño, sección 5. La colisión real sigue
// decidiéndose siempre por AABB, el semáforo no la reemplaza.

function estadoSemaforo(semaforo, ahora) {
  const transcurrido = ahora - tiempoInicioNivel;
  const fase = Math.floor(transcurrido / semaforo.cicloMs) % 2;
  // fase 0: autos circulan (rojo peatón) -- fase 1: autos detenidos (verde peatón)
  return fase === 0 ? 'autosVerde' : 'autosRojo';
}

function carrilDetenido(carrilIndex, ahora) {
  return semaforosInfo.some(
    (s) => s.carrilAsociado === carrilIndex && estadoSemaforo(s, ahora) === 'autosRojo'
  );
}

// ---- Movimiento de autos (continuo, en píxeles) ----

function actualizarAutos() {
  if (obstaculosCache.length === 0) return;

  const ahora = performance.now();
  const movimientos = [];

  obstaculosCache.forEach((obstaculo, index) => {
    const info = autosInfo[index];
    if (!info) return;

    if (carrilDetenido(info.carrilIndex, ahora)) return; // semáforo en rojo: este auto no se mueve

    let dx = info.direccion * info.velocidad;
    let nuevaX = obstaculo.x + dx;

    if (info.direccion > 0 && nuevaX > canvas.width) {
      nuevaX = -obstaculo.ancho;
      dx = nuevaX - obstaculo.x;
    } else if (info.direccion < 0 && nuevaX + obstaculo.ancho < 0) {
      nuevaX = canvas.width;
      dx = nuevaX - obstaculo.x;
    }

    obstaculo.x = nuevaX;
    movimientos.push({ index, dx, dy: 0 });
  });

  if (movimientos.length > 0) {
    window.api.moverObstaculos(movimientos);
  }
}

// ---- Colisión ----

async function verificarColision() {
  if (enInvulnerabilidad || procesandoColision) return;
  procesandoColision = true;

  try {
    const hayColision = await window.api.detectarColision();
    if (!hayColision) return;

    enInvulnerabilidad = true;
    const estadoJugador = await window.api.perderVida();
    pintarVidas(estadoJugador.vidas);

    if (!estadoJugador.estaVivo) {
      await cargarNivel(NIVELES[indiceNivelActual]);
    } else {
      setTimeout(() => {
        enInvulnerabilidad = false;
      }, INVULNERABILIDAD_MS);
    }
  } finally {
    procesandoColision = false;
  }
}

// ---- Meta / fin de nivel ----
// La meta es un concepto que vive solo en el JSON (game.js), nunca se le
// pasa al addon: Escenario (C++) no tiene campo de jugador ni de meta, asi
// que Nivel::esCompletado() no puede estar comparando esto -- se verifica
// aqui comparando celda actual vs metaActual (ver doc de diseño, sección 7).

async function verificarMeta() {
  if (!metaActual) return;
  if (celda.col !== metaActual.col || celda.fila !== metaActual.fila) return;

  await manejarNivelCompletado();
}

async function manejarNivelCompletado() {
  const siguienteIndice = indiceNivelActual + 1;

  if (siguienteIndice < NIVELES.length) {
    indiceNivelActual = siguienteIndice;
    await cargarNivel(NIVELES[indiceNivelActual]);
  } else {
    // MVP: todavía no existen nivel2.json / nivel3.json. Placeholder
    // simple hasta que se diseñen -- reemplazar por una pantalla de
    // victoria real cuando estén listos.
    alert('¡Nivel completado! Todavía no hay más niveles diseñados.');
    await cargarNivel(NIVELES[indiceNivelActual]);
  }
}

// ---- Input del jugador ----

const TECLAS = {
  ArrowUp: { dCol: 0, dFila: -1 },
  ArrowDown: { dCol: 0, dFila: 1 },
  ArrowLeft: { dCol: -1, dFila: 0 },
  ArrowRight: { dCol: 1, dFila: 0 },
};

async function manejarTecla(event) {
  const delta = TECLAS[event.key];
  if (!delta) return;

  const ahora = performance.now();
  if (ahora - ultimoMovimiento < COOLDOWN_MS) return;

  const nuevaCol = celda.col + delta.dCol;
  const nuevaFila = celda.fila + delta.dFila;

  if (nuevaCol < 0 || nuevaCol >= COLS || nuevaFila < 0 || nuevaFila >= ROWS) {
    return;
  }

  ultimoMovimiento = ahora;
  celda.col = nuevaCol;
  celda.fila = nuevaFila;

  const dxPx = delta.dCol * CELL;
  const dyPx = delta.dFila * CELL;
  await window.api.moverJugador(dxPx, dyPx);

  await verificarMeta();
}

window.addEventListener('keydown', manejarTecla);

// ---- Puntaje ----

function pintarPuntaje(estadoPuntaje) {
  puntajeValorEl.textContent = estadoPuntaje.puntos;
}

function pintarVidas(vidas) {
  vidasValorEl.textContent = vidas;
}

async function actualizarPuntaje(puntos) {
  const estado = await window.api.sumarPuntos(puntos);
  pintarPuntaje(estado);
}

// ---- Dibujo ----

function dibujarMeta() {
  if (!metaActual) return;

  const x = metaActual.col * CELL;
  const y = metaActual.fila * CELL;
  const centroX = x + CELL / 2;

  // Asta de la bandera
  ctx.strokeStyle = '#e0e0e0';
  ctx.lineWidth = 2;
  ctx.beginPath();
  ctx.moveTo(centroX, y + CELL - 4);
  ctx.lineTo(centroX, y + 4);
  ctx.stroke();

  // Tela de la bandera (triángulo), verde para distinguirla claramente
  // de los autos (rojo) y el jugador (celeste).
  ctx.fillStyle = '#2ecc71';
  ctx.beginPath();
  ctx.moveTo(centroX, y + 4);
  ctx.lineTo(centroX + 16, y + 10);
  ctx.lineTo(centroX, y + 16);
  ctx.closePath();
  ctx.fill();
}

function dibujarSemaforos() {
  const ahora = performance.now();

  semaforosInfo.forEach((s) => {
    const x = s.col * CELL;
    const y = s.fila * CELL;
    const centroX = x + CELL / 2;
    const estado = estadoSemaforo(s, ahora);

    // Cuerpo del semáforo (poste + caja)
    ctx.fillStyle = '#1a1f2a';
    ctx.fillRect(centroX - 6, y + 4, 12, 24);

    // Luz roja (arriba) y verde (abajo), referidas siempre a los AUTOS
    // -- coherente con la tabla del doc de diseño, sección 5.
    const activo = estado === 'autosRojo';
    ctx.beginPath();
    ctx.arc(centroX, y + 10, 4, 0, Math.PI * 2);
    ctx.fillStyle = activo ? '#ff3b3b' : 'rgba(255, 59, 59, 0.25)';
    ctx.fill();

    ctx.beginPath();
    ctx.arc(centroX, y + 22, 4, 0, Math.PI * 2);
    ctx.fillStyle = !activo ? '#2ecc71' : 'rgba(46, 204, 113, 0.25)';
    ctx.fill();
  });
}

function dibujar() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  dibujarMeta();
  dibujarSemaforos();

  ctx.fillStyle = '#ff5050';
  obstaculosCache.forEach((o) => {
    if (o.activo) ctx.fillRect(o.x, o.y, o.ancho, o.alto);
  });

  ctx.globalAlpha = enInvulnerabilidad ? 0.4 : 1;
  ctx.fillStyle = '#00c8ff';
  ctx.fillRect(celda.col * CELL, celda.fila * CELL, CELL, CELL);
  ctx.globalAlpha = 1;
}

// ---- Loop principal ----

let ultimoTimestamp = null;

function loop(timestamp) {
  if (ultimoTimestamp === null) ultimoTimestamp = timestamp;
  ultimoTimestamp = timestamp;

  actualizarAutos();
  verificarColision();
  dibujar();

  requestAnimationFrame(loop);
}

cargarNivel(NIVELES[indiceNivelActual]);
requestAnimationFrame(loop);