// Punto de entrada del programa
// inicializa subsistemas y ejecuta el bucle principal

#include <iostream>
#include "Game.h"
#include "spdlog/spdlog.h"

int main(int argc, char* argv[]) {
    
    spdlog::set_level(spdlog::level::info); 
    spdlog::info("Iniciando aplicación...");

    try {
        auto game = std::make_unique<Game>();
        if (game->initialize() != 0) {
            spdlog::critical("Fallo fatal al inicializar el juego.");
            return -1;
        }
        game->run();
        game->shutdown();
    }
    catch (const std::exception& e) {
        spdlog::critical("Excepción fatal no controlada: {}", e.what());
        return -1;
    }
    catch (...) {
        spdlog::critical("Excepción fatal desconocida.");
        return -1;
    }

    spdlog::info("Aplicación cerrada limpiamente.");
    return 0;
}