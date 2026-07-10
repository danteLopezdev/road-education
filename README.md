# Road Education

Videojuego educativo de cruce vial desarrollado como Evaluación Continua 3 (EC3) del curso de Lenguaje de Programación I, de la carrera de Ingeniería de Software.

## Descripción del proyecto

**Road Education** es un videojuego de estilo *Frogger* orientado a la concientización sobre seguridad vial. El jugador debe conducir a su personaje a través de distintos carriles de tráfico, esquivando vehículos y respetando semáforos, hasta alcanzar la meta en cada nivel. El proyecto integra una capa de lógica de negocio desarrollada en C++ (compilada como addon nativo de Node.js) con una capa de presentación e interacción construida en JavaScript, HTML5 Canvas y el framework Electron.

El desarrollo del proyecto buscó aplicar los conceptos fundamentales del curso —estructuras de control, funciones, arreglos, y en particular la integración de un lenguaje compilado con un entorno de ejecución JavaScript mediante N-API— dentro de un caso de aplicación práctico y jugable.

## Objetivos

- Aplicar los fundamentos de programación estructurada y orientada a objetos en un proyecto funcional de escritorio.
- Implementar comunicación entre un módulo nativo en C++ y una interfaz gráfica en JavaScript mediante N-API.
- Diseñar un sistema de niveles configurable y escalable a partir de archivos JSON.
- Empaquetar y distribuir una aplicación de escritorio multiplataforma mediante Electron.

## Tecnologías utilizadas

| Componente | Tecnología |
|---|---|
| Lógica de negocio (backend nativo) | C++ |
| Enlace entre C++ y Node.js | N-API |
| Interfaz y renderizado | JavaScript, HTML5 Canvas |
| Framework de aplicación de escritorio | Electron |
| Configuración de niveles | JSON |
| Empaquetado / distribución | Electron Packager / Builder (.exe) |

## Arquitectura del proyecto

El proyecto sigue una arquitectura de dos capas:

**1. Capa nativa (C++)**
Contiene la lógica de negocio del juego, implementada mediante las siguientes clases:

- `Jugador`: gestiona la posición y el estado del personaje controlado por el usuario.
- `Puntaje`: administra el sistema de puntuación (incremento por avance, penalización por colisión, bono por nivel completado).
- `Escenario`: representa la configuración del nivel en ejecución (carriles, dirección del tráfico, semáforos).
- `Obstaculo`: modela a los vehículos que circulan por los carriles.
- `Colisiones`: contiene la lógica de detección de colisiones entre el jugador y los obstáculos.
- `Validador`: valida las condiciones de finalización de nivel (meta alcanzada) y las reglas del juego.

Esta capa se compila como un módulo nativo (`road_education_native.node`) y se comunica con la capa de interfaz mediante bindings de N-API expuestos a través de los manejadores IPC definidos en `main.js` y `preload.js`.

**2. Capa de interfaz (JavaScript / Electron)**
Contiene toda la lógica de renderizado, entrada de usuario y flujo del juego, implementada principalmente en `scripts/game.js`:

- Sistema de movimiento en grilla y captura de eventos de teclado.
- Sistema de sprites con soporte de imágenes y figuras SVG de respaldo, incluyendo volteo direccional para carriles con tráfico en sentido opuesto.
- Dibujo procedural de carriles a partir de la configuración de cada nivel (líneas divisorias, cruce peatonal).
- Bucle de movimiento y reciclaje de obstáculos por carril.
- Integración del sistema de puntaje y de las reglas de colisión provenientes de la capa nativa.
- Lógica de semáforos y de finalización de nivel.
- Pantalla de victoria con opción de reiniciar o salir del juego.

## Niveles

El juego incluye tres niveles con dificultad progresiva, definidos en archivos de configuración JSON independientes:

1. **Nivel 1**: introducción a la mecánica básica de cruce.
2. **Nivel 2 — "Doble cruce"**: dos cruces con carriles de tráfico en direcciones opuestas.
3. **Nivel 3 — "Triple carril, un cruce libre"**: tres carriles, donde el carril central no cuenta con semáforo.

## Estructura del proyecto

```
road-education/
├── road-education-native/
│   ├── main.js              # Proceso principal de Electron
│   ├── preload.js           # Puente IPC entre el proceso principal y el renderer
│   ├── package.json         # Configuración del proyecto Node/Electron
│   ├── build/                # Addon nativo compilado (road_education_native.node)
│   └── src/                 # Código fuente en C++ (clases del dominio)
├── scripts/
│   └── game.js               # Lógica del juego en el renderer
├── niveles/
│   ├── nivel1.json
│   ├── nivel2.json
│   └── nivel3.json
├── assets/
│   └── sprites/              # Recursos gráficos (car.svg, player.svg, etc.)
└── README.md
```

## Instalación y ejecución

### Requisitos previos

- Node.js (versión LTS recomendada)
- Herramientas de compilación de C++ (Visual Studio Build Tools en Windows, o su equivalente según el sistema operativo)

### Pasos

```bash
# Clonar el repositorio
git clone https://github.com/danteLopezdev/road-education.git
cd road-education/road-education-native

# Instalar dependencias
npm install

# Compilar el addon nativo (si es necesario)
npm run build

# Ejecutar la aplicación en modo desarrollo
npm start
```

### Ejecutable

El proyecto también se encuentra disponible como aplicación empaquetada (`.exe`) para sistemas Windows, generada mediante Electron. El ejecutable fue probado satisfactoriamente en distintos equipos por los integrantes del equipo de desarrollo.

## Controles

| Tecla | Acción |
|---|---|
| ↑ / W | Avanzar |
| ↓ / S | Retroceder |
| ← / A | Mover a la izquierda |
| → / D | Mover a la derecha |

## Autores

Trabajo elaborado por los integrantes del equipo del curso de Lenguaje de Programación I:

- **López Flores, Dante**
- **Castillo Benítez, Lutber Saúl**
- **Villegas Ancajima, Marlon**

## Información académica

- **Universidad:** Universidad Científica del Sur (UCSUR)
- **Facultad / Carrera:** Ingeniería de Software
- **Curso:** Lenguaje de Programación I
- **Ciclo:** III
- **Evaluación:** EC3 — Proyecto Final

## Licencia

Proyecto desarrollado con fines académicos en el marco del curso de Lenguaje de Programación I de la Universidad Científica del Sur. Su uso y distribución quedan sujetos a lo dispuesto por los autores y la institución.