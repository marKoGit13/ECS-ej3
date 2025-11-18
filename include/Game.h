// Gestión principal del juego: ventana, renderer y recursos
// controla inicialización y bucle principal

#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <memory>
#include <map>
#include "World.h"

class Game {
public:
    Game();
    ~Game();
    int initialize();
    void run();
    void shutdown();

private:
    void processInput();
    bool loadTexture(const std::string& id, const std::string& path);
    bool loadFont(const std::string& id, const std::string& path, int size);

    SDL_Window* mWindow = nullptr;
    SDL_Renderer* mRenderer = nullptr;

    std::map<std::string, SDL_Texture*> mTextureMap;
    std::map<std::string, TTF_Font*> mFontMap;

    bool mIsRunning = false;
    bool mIsGameOver = false;
    Uint64 mTicksLastFrame = 0;
    int mEnemiesSpawned = 0;

    std::unique_ptr<World> mWorld;
};