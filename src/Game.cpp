// Implementación de inicialización, bucle y limpieza del juego
// carga recursos y arranca el mundo

#include "Game.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <SDL2/SDL_surface.h> // Para SDL_FreeSurface

// Componentes
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "HealthComponent.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"

// Sistemas
#include "PlayerInputSystem.h"
#include "MovementSystem.h"
#include "CollisionSystem.h"
#include "DamageSystem.h"
#include "SpawnSystem.h"
#include "RenderSystem.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

Game::Game() {}
Game::~Game() {}

bool Game::loadTexture(const std::string& id, const std::string& path) {
    SDL_Texture* texture = IMG_LoadTexture(mRenderer, path.c_str());
    if (texture == nullptr) {
        spdlog::error("Fallo al cargar textura: {} - Error: {}", path, IMG_GetError());
        return false;
    }
    mTextureMap[id] = texture;
    return true;
}

bool Game::loadFont(const std::string& id, const std::string& path, int size) {
    TTF_Font* font = TTF_OpenFont(path.c_str(), size);
    if (font == nullptr) {
        spdlog::error("Fallo al cargar fuente: {} - Error: {}", path, TTF_GetError());
        return false;
    }
    mFontMap[id] = font;
    return true;
}

int Game::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        spdlog::error("Error al inicializar SDL: {}", SDL_GetError());
        return -1;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        spdlog::error("Error al inicializar SDL_image: {}", IMG_GetError());
        return -1;
    }
    if (TTF_Init() != 0) {
        spdlog::error("Error al inicializar SDL_ttf: {}", TTF_GetError());
        return -1;
    }

    mWindow = SDL_CreateWindow(
        "Juego de Naves - Ejercicio 3",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_FULLSCREEN_DESKTOP
    );
    if (mWindow == nullptr) {
        spdlog::error("Error al crear ventana: {}", SDL_GetError());
        return -1;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (mRenderer == nullptr) {
        spdlog::error("Error al crear renderer: {}", SDL_GetError());
        return -1;
    }

    // Cargar Assets
    if (!loadTexture("player", "assets/player.png")) return -1;
    if (!loadTexture("enemy", "assets/enemy.png")) return -1;
    if (!loadTexture("background", "assets/background.jpg")) return -1;
    if (!loadFont("main_font", "assets/arial.ttf", 24)) return -1;
    if (!loadFont("game_over_font", "assets/arial.ttf", 48)) return -1;

    // Crear Mundo y Sistemas
    mWorld = std::make_unique<World>();
    
    mWorld->addSystem(std::make_unique<PlayerInputSystem>(mIsGameOver,300.0f));
    mWorld->addSystem(std::make_unique<MovementSystem>(mIsGameOver, SCREEN_WIDTH, SCREEN_HEIGHT));
    mWorld->addSystem(std::make_unique<CollisionSystem>(mIsGameOver));
    mWorld->addSystem(std::make_unique<DamageSystem>(mIsGameOver));
    mWorld->addSystem(std::make_unique<SpawnSystem>(
        mIsGameOver, 
        mTextureMap["enemy"], 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT,
        mEnemiesSpawned
    ));
    mWorld->addSystem(std::make_unique<RenderSystem>(
        mRenderer, 
        mFontMap["main_font"], 
        mFontMap["game_over_font"],
        mIsGameOver,
        mEnemiesSpawned,
        mTextureMap["background"]
    ));

    // Crear Entidad Jugador
    auto& player = mWorld->createEntity();
    player.addComponent<PlayerComponent>();
    player.addComponent<TransformComponent>(
        glm::vec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f), // Posición
        glm::vec2(0.0f),                                      // Velocidad (se establece en PlayerInputSystem)
        glm::vec2(0.1f)                                       // Escala 
    );
    player.addComponent<HealthComponent>(3); // 3 vidas
    
    int w, h;
    SDL_QueryTexture(mTextureMap["player"], nullptr, nullptr, &w, &h);
    player.addComponent<SpriteComponent>(mTextureMap["player"], w, h);
    player.addComponent<ColliderComponent>(0, 0, w, h); 

    mIsRunning = true;
    mIsGameOver = false;
    mTicksLastFrame = SDL_GetTicks64();
    
    spdlog::info("Juego inicializado correctamente.");
    return 0;
}

void Game::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                mIsRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    mIsRunning = false;
                }
                if (mIsGameOver && event.key.keysym.sym == SDLK_RETURN) {
                    mIsRunning = false;
                }
                break;
        }
    }
}

void Game::run() {
    while (mIsRunning) {
        while (!SDL_TICKS_PASSED(SDL_GetTicks64(), mTicksLastFrame + 16));
        
        float dt = (SDL_GetTicks64() - mTicksLastFrame) / 1000.0f;
        if (dt > 0.05f) dt = 0.05f; // Limitar el delta time
        
        mTicksLastFrame = SDL_GetTicks64();
        
        processInput();
        mWorld->update(dt);
        mWorld->refresh(); // Limpiar entidades destruidas
    }
}

void Game::shutdown() {
    spdlog::info("Cerrando el juego...");
    
    for (auto& pair : mTextureMap) {
        SDL_DestroyTexture(pair.second);
    }
    for (auto& pair : mFontMap) {
        TTF_CloseFont(pair.second);
    }
    
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
