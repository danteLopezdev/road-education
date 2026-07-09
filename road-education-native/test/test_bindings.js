// test_bindings.js
// Prueba manual de los bindings ya implementados. No reemplaza a
// pruebas_core.cpp (que prueba la logica C++ pura, sin Node), sino que
// confirma que la capa N-API expone y conecta bien esa logica.
//
// Uso: npm run build && npm test

const native = require('../index.js');

let fallos = 0;
function verificar(condicion, nombre) {
    console.log(`[${condicion ? 'OK  ' : 'FAIL'}] ${nombre}`);
    if (!condicion) { fallos++; process.exitCode = 1; }
}

console.log('=== Carga del addon ===');
verificar(typeof native.JugadorNativo === 'function', 'JugadorNativo esta expuesto');
verificar(typeof native.ObstaculoNativo === 'function', 'ObstaculoNativo esta expuesto');
verificar(typeof native.EscenarioNativo === 'function', 'EscenarioNativo esta expuesto');
verificar(typeof native.PuntajeNativo === 'function', 'PuntajeNativo esta expuesto');

console.log('\n=== PuntajeNativo ===');
const p = new native.PuntajeNativo();
verificar(p.nivel === 1 && p.puntos === 0, 'inicia en nivel 1 con 0 puntos');
p.sumar(50);
verificar(p.puntos === 50, 'sumar(50) deja puntos en 50');
p.restarVida();
p.reset();
verificar(p.puntos === 0, 'reset() vuelve a 0 puntos');

console.log('\n=== JugadorNativo ===');
const j = new native.JugadorNativo(10, 20);
verificar(j.x === 10 && j.y === 20, 'se crea con x,y iniciales');
verificar(j.vidas === 3, 'inicia con MAX_VIDAS (3)');
j.mover(5, -3);
verificar(j.x === 15 && j.y === 17, 'mover(5, -3) suma a x,y');
const vidasAntes = j.vidas;
j.perderVida();
verificar(j.vidas === vidasAntes - 1, 'perderVida() resta una vida');
j.resetPosicion();
verificar(j.x === 10 && j.y === 20, 'resetPosicion() vuelve al spawn');

console.log('\n=== ObstaculoNativo ===');
const o = new native.ObstaculoNativo(10, 20, 30, 30, 'hueco');
verificar(o.tipo === 'hueco' && o.activo === true, 'se crea con tipo y activo=true');
o.desactivar();
verificar(o.activo === false, 'desactivar() funciona');
o.activar();
verificar(o.activo === true, 'activar() funciona');
verificar(typeof o.colisionaCon(j) === 'boolean', 'colisionaCon(jugador) devuelve boolean');

console.log('\n=== FisicaBindings / ColisionesBindings ===');
verificar(native.calcularVelocidad(0, 10, 2) === 20, 'calcularVelocidad(0,10,2) === 20');
native.aplicarGravedad(j);
native.calcularPosicion(j, 0.016);
verificar(typeof native.detectarAABB(j, o) === 'boolean', 'detectarAABB devuelve boolean');

console.log('\n=== EscenarioNativo / NivelBindings ===');
const esc = new native.EscenarioNativo(1, 'Cruce peatonal');
esc.agregarObstaculo(o);
const activos = esc.obstaculosActivos();
verificar(Array.isArray(activos) && activos.length === 1, 'obstaculosActivos() devuelve 1 elemento');
verificar(activos[0] instanceof native.ObstaculoNativo, 'cada elemento es un ObstaculoNativo real');
esc.limpiar();
verificar(esc.obstaculosActivos().length === 0, 'limpiar() vacia el escenario');

let lanzoRangeError = false;
try { native.cargarEscenario(-1); } catch (e) { lanzoRangeError = e instanceof RangeError; }
verificar(lanzoRangeError, 'cargarEscenario(-1) lanza RangeError (id invalido)');

const escCargado = native.cargarEscenario(1);
verificar(escCargado instanceof native.EscenarioNativo, 'cargarEscenario(1) devuelve un EscenarioNativo');
verificar(typeof native.esCompletado(escCargado) === 'boolean', 'esCompletado() devuelve boolean');

console.log(fallos === 0 ? '\nTodos los bindings funcionan.' : `\n${fallos} prueba(s) fallaron.`);
