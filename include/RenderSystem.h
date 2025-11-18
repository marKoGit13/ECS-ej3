#pragma once
#include "ISystem.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class RenderSystem : public ISystem {
public:
    RenderSystem(SDL_Renderer* renderer, TTF_Font* font, TTF_Font* gameOverFont, bool& isGameOverRef, int& enemiesSpawned, SDL_Texture* backgroundTexture);
    ~RenderSystem(); 
    void update(World& world, float dt) override;

private:
    SDL_Renderer* mRenderer; 
    TTF_Font* mFont;         
    bool& mIsGameOver;       
    int& mEnemiesSpawned;
    SDL_Texture* mBackgroundTexture;
    TTF_Font* mGameOverFont; 
    SDL_Texture* mTimeTexture = nullptr;
    SDL_Rect mTimeRect;
    SDL_Texture* mEnemyCountTexture = nullptr;
    SDL_Rect mEnemyCountRect;
    SDL_Texture* mGameOverTexture = nullptr;
    SDL_Rect mGameOverRect;
    
    float mTotalTime = 0.0f;
    
    // Constantes para centrar el texto de Game Over
    const int SCREEN_WIDTH = 1024;
    const int SCREEN_HEIGHT = 768;
};