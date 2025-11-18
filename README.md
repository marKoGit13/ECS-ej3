# Juego de Supervivencia 2D con Arquitectura ECS en C++

Este proyecto es un juego de supervivencia 2D funcional construido desde cero en C++. El núcleo del motor se basa en el patrón de diseño **Entity-Component-System (ECS)**, una arquitectura moderna que separa los datos del comportamiento.

La demo incluida es un juego de supervivencia donde el jugador (controlado con `WASD`) debe esquivar a un enjambre creciente de enemigos que lo persiguen. El objetivo es sobrevivir el mayor tiempo posible.

## Características
- **Arquitectura ECS:** Diseño modular donde la lógica (Sistemas) itera sobre los datos (Componentes).
- **Manejo de Memoria Moderno:** Gestión automática y segura de la memoria usando **Smart Pointers** (`std::unique_ptr`).
- **Bus de Eventos:** Un sistema de cola desacoplado (`emit`/`poll`) para manejar la comunicación entre sistemas (ej. `CollisionSystem` envía `DamageEvent`, `DamageSystem` lo recibe).
- **Renderizado 2D:** Utiliza **SDL2**, con soporte para imágenes (`SDL_image`), texto (`SDL_ttf`), un fondo de pantalla y modo de pantalla completa.
- **Jugabilidad y Lógica:**
    - Control del jugador por teclado (`WASD`).
    - Detección de colisiones AABB que respeta la escala del sprite.
    - Lógica de persecución simple en `SpawnSystem`.
    - Aparición de enemigos con velocidad aleatoria.
    - Sistema de `HealthComponent` con invencibilidad temporal (iframes).
- **UI en Tiempo Real:** Muestra el tiempo transcurrido, una barra de vida sobre el jugador y el conteo de enemigos.

---

## Diseño y Arquitectura

La estructura del proyecto aplica una separación clara de intereses, una piedra angular del diseño de software moderno y del paradigma ECS.

### El Patrón Entity-Component-System
- **`Component`:** Clases/structs de datos puros que no contienen lógica (ej. `TransformComponent`, `HealthComponent`).
- **`Entity`:** Actúa como un contenedor y "dueño" de sus componentes. Utiliza `std::unique_ptr` para la memoria y un `std::map<std::type_index, Component*>` para acceso rápido (`O(log n)`) a los componentes por tipo, evitando `dynamic_cast`.
- **`ISystem`:** Interfaz base para toda la lógica del juego (ej. `MovementSystem`, `CollisionSystem`).
- **`World`:** Es el "dueño" de todos los `std::unique_ptr<Entity>` y `std::unique_ptr<ISystem>`. Maneja las actualizaciones y gestiona la cola del bus de eventos.
- **`Game`:** Es la clase principal que encapsula el bucle del juego (`processInput`, `run`), el estado (`mIsGameOver`) y la inicialización/cierre de SDL.

### Buenas Prácticas Implementadas
- **Manejo de Memoria (Smart Pointers):** Se sigue una estricta jerarquía de propiedad: `Game` posee `World`, `World` posee `Entity`s y `ISystem`s, y `Entity` posee `Component`s. El uso de `std::unique_ptr` garantiza la adherencia a los principios de **RAII** (Resource Acquisition Is Initialization), liberando la memoria automáticamente y previniendo fugas.
- **Código Desacoplado:** Los sistemas se comunican indirectamente. Por ejemplo, `CollisionSystem` no reduce la vida del jugador; emite un `DamageEvent` genérico. `DamageSystem` reacciona a este evento y aplica el daño al jugador, sin saber quién lo causó.

### Dependencias y Compilación
- **Librerías Header-Only:** `GLM` y `spdlog` se utilizan directamente desde sus cabeceras.
    - *Nota de compilación:* Se usa el flag `-DSPDLOG_WIN_NO_FILE_FLUSH` para asegurar la compatibilidad de `spdlog` con MinGW.
- **Librerías Dinámicas:** `SDL2`, `SDL2_image` y `SDL2_ttf` se enlazan dinámicamente, como es estándar.
- **Automatización:** Todo el proceso de compilación, enlazado y gestión de dependencias está automatizado a través de un `Makefile` explícito.

---

## Cómo Compilar y Ejecutar

### Requisitos
- Compilador `g++` (el `Makefile` está configurado para `C:\MinGW`)
- `make`
- Una terminal MINGW64 (como Git Bash)

### Compilación
- Se recomienda primero hacer:
```sh
make clean
```
Para limpiar archivos y tener una buena compilación y ejecución

Abre una terminal en la raíz del proyecto y ejecuta:
```sh
make all
```
El Makefile compilará todos los archivos .cpp de la carpeta src/, creará los objetos en build/, enlazará el ejecutable final en bin/juego.exe y copiará las .dll necesarias a la carpeta bin/.

### Ejecución
Para iniciar el juego:
```sh
make run
```
O directamente:
```sh
./bin/juego.exe
```
### Limpieza
Para eliminar los archivos generados:
```sh
make clean
```