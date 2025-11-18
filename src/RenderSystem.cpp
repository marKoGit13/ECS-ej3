// Implementación del renderizado y gestión de recursos gráficos
// dibuja entidades, UI y texto

#include "RenderSystem.h"
#include "World.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "HealthComponent.h" 
#include "PlayerComponent.h"
#include "spdlog/spdlog.h"
#include <SDL2/SDL_surface.h> // Para SDL_FreeSurface
#include <string>
#include <iomanip> 
#include <sstream> 

RenderSystem::RenderSystem(SDL_Renderer* renderer, TTF_Font* font, TTF_Font* gameOverFont, bool& isGameOverRef, int& enemiesSpawned, SDL_Texture* backgroundTexture)
    : mRenderer(renderer), 
      mFont(font),
      mGameOverFont(gameOverFont),
      mIsGameOver(isGameOverRef),
      mEnemiesSpawned(enemiesSpawned),
      mBackgroundTexture(backgroundTexture), 
      mTotalTime(0.0f) {

    if (mRenderer == nullptr) spdlog::error("RenderSystem: SDL_Renderer es nulo");
    if (mFont == nullptr) spdlog::error("RenderSystem: TTF_Font (main) es nulo");
    if (mGameOverFont == nullptr) spdlog::error("RenderSystem: TTF_Font (game over) es nulo");
    
    mTimeRect = { 10, 10, 0, 0 }; // Posición UI Tiempo
    mEnemyCountRect = { 10, 40, 0, 0 }; // Posición UI Enemigos
}

RenderSystem::~RenderSystem() {
    if (mTimeTexture) {
        SDL_DestroyTexture(mTimeTexture);
    }
    if (mGameOverTexture) {
        SDL_DestroyTexture(mGameOverTexture);
    }
    if (mEnemyCountTexture) {
        SDL_DestroyTexture(mEnemyCountTexture);
    }
}

void RenderSystem::update(World& world, float dt) {
    mTotalTime += dt;

    if (mIsGameOver) {
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255); 
    } else {
        SDL_SetRenderDrawColor(mRenderer, 20, 20, 30, 255); 
    }
    SDL_RenderClear(mRenderer);

    if (!mIsGameOver && mBackgroundTexture) {
        SDL_RenderCopy(mRenderer, mBackgroundTexture, nullptr, nullptr);
    }

    if (mIsGameOver) {
        if (mGameOverTexture == nullptr) { 
            std::stringstream ss;
            ss << "Tiempo final: " << std::fixed << std::setprecision(2) << (mTotalTime - dt);
            ss << " | Enemigos atraidos: " << mEnemiesSpawned;
            std::string gameOverText = ss.str();

            SDL_Color white = {255, 255, 255, 255};
            SDL_Surface* surface = TTF_RenderText_Blended(mGameOverFont, gameOverText.c_str(), white);
            
            if (surface) {
                mGameOverTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
                mGameOverRect.w = surface->w;
                mGameOverRect.h = surface->h;
                mGameOverRect.x = (SCREEN_WIDTH - surface->w) / 2;
                mGameOverRect.y = (SCREEN_HEIGHT - surface->h) / 2;
                SDL_FreeSurface(surface); 
            } else {
                spdlog::error("No se pudo crear la superficie de texto de Game Over: {}", TTF_GetError());
            }
        }
        
        if (mGameOverTexture) {
            SDL_FRect dest = { (float)mGameOverRect.x, (float)mGameOverRect.y, (float)mGameOverRect.w, (float)mGameOverRect.h };
            SDL_RenderCopyF(mRenderer, mGameOverTexture, nullptr, &dest);
        }

    } else {
        // Dibujar Sprites
        for (auto& entity : world.getEntities()) {
            if (!entity->hasComponent<SpriteComponent>() || !entity->hasComponent<TransformComponent>()) {
                continue;
            }

            auto sprite = entity->getComponent<SpriteComponent>();
            auto transform = entity->getComponent<TransformComponent>();

            if (entity->hasComponent<HealthComponent>() && entity->getComponent<HealthComponent>()->isInvincible()) {
                if (static_cast<int>(mTotalTime * 10.0f) % 2 == 0) continue;
            }

            SDL_FRect destRect = {
                transform->position.x,
                transform->position.y,
                static_cast<float>(sprite->width) * transform->scale.x,
                static_cast<float>(sprite->height) * transform->scale.y
            };
            SDL_RenderCopyF(mRenderer, sprite->texture, &sprite->srcRect, &destRect);

            //barra con vida
            if (entity->hasComponent<PlayerComponent>() && entity->hasComponent<HealthComponent>()) {
                auto health = entity->getComponent<HealthComponent>();
                
                float healthPercent = static_cast<float>(health->hp) / static_cast<float>(health->maxHp);
                int barWidth = static_cast<int>(destRect.w); // Ancho de la barra = ancho del sprite
                int barHeight = 5; // 5 píxeles de alto
                int barX = static_cast<int>(destRect.x);
                int barY = static_cast<int>(destRect.y) - barHeight - 5; // 5 píxeles arriba del sprite

                // Dibujar fondo rojo
                SDL_Rect bgBar = { barX, barY, barWidth, barHeight };
                SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
                SDL_RenderFillRect(mRenderer, &bgBar);

                // Dibujar vida verde
                SDL_Rect hpBar = { barX, barY, static_cast<int>(barWidth * healthPercent), barHeight };
                SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
                SDL_RenderFillRect(mRenderer, &hpBar);
            }
        }

        // Dibujar UI (Tiempo)
        std::stringstream timeStream;
        timeStream << "Tiempo: " << std::fixed << std::setprecision(2) << mTotalTime;
        std::string timeText = timeStream.str();

        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* timeSurface = TTF_RenderText_Blended(mFont, timeText.c_str(), white);
        
        if (timeSurface) {
            if (mTimeTexture) SDL_DestroyTexture(mTimeTexture); 
            mTimeTexture = SDL_CreateTextureFromSurface(mRenderer, timeSurface);
            mTimeRect.w = timeSurface->w;
            mTimeRect.h = timeSurface->h;
            SDL_FreeSurface(timeSurface); 

            SDL_FRect timeDestRect = { (float)mTimeRect.x, (float)mTimeRect.y, (float)mTimeRect.w, (float)mTimeRect.h };
            SDL_RenderCopyF(mRenderer, mTimeTexture, nullptr, &timeDestRect);
        } else {
            spdlog::error("No se pudo crear la superficie de texto de tiempo: {}", TTF_GetError());
        }

        //Dibujar UI (Enemigos)
        std::stringstream enemyStream;
        enemyStream << "Enemigos: " << mEnemiesSpawned;
        std::string enemyText = enemyStream.str();

        SDL_Surface* enemySurface = TTF_RenderText_Blended(mFont, enemyText.c_str(), white);
        if (enemySurface) {
            if (mEnemyCountTexture) SDL_DestroyTexture(mEnemyCountTexture);
            mEnemyCountTexture = SDL_CreateTextureFromSurface(mRenderer, enemySurface);
            mEnemyCountRect.w = enemySurface->w;
            mEnemyCountRect.h = enemySurface->h;
            SDL_FreeSurface(enemySurface);

            SDL_FRect enemyDestRect = { (float)mEnemyCountRect.x, (float)mEnemyCountRect.y, (float)mEnemyCountRect.w, (float)mEnemyCountRect.h };
            SDL_RenderCopyF(mRenderer, mEnemyCountTexture, nullptr, &enemyDestRect);
        } else {
            spdlog::error("No se pudo crear la superficie de texto de enemigos: {}", TTF_GetError());
        }
    }

    SDL_RenderPresent(mRenderer);
}