#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <algorithm>
#include <vector>
#include <iostream>

#include "Math.hpp"
#include "Bullet.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "WindowRenderer.hpp"
#include "Button.hpp"
#include "Menu.hpp"

#define GAME_MAIN_MENU 0
#define GAME_PLAYING 1
#define GAME_PAUSE 2
#define GAME_END 3

class Game{
    public: 
        Game(WindowRenderer& renderer);
        bool isRunning() const;
        bool hasClickedStart() const;
        int getState() const;
        void changeState(int state);

        void addStar(const Entity& star);
        void addBullet(const Bullet& bullet);

        std::vector<Entity> getStarList() const;
        std::vector<Bullet> getBulletList() const;
        Player getPlayer() const;
        Menu* getMainMenu();

        void startNewGame(WindowRenderer& renderer);
        void processGameEvents(WindowRenderer& renderer, const InputHandler& inputHandler, const float& deltaTime);
        void renderMainMenu(WindowRenderer& renderer);
        void renderPlaying(WindowRenderer& renderer);
        void endGame();
        void closeGame();

    private:
        bool gameRunning = 0;
        int gameState = 0;
        int gameScore = 0;
        Player character;
        std::vector<Bullet> bulletList;
        std::vector<Entity> starList;
        
        uint32_t gameOpened = 0;
        uint32_t gameStartedPlaying = 0;
        uint32_t prevGameRenderTime = 0;
        Sint32 gameHighscore;

        Menu mainMenu;
        Button startButton;
        Button quitButton;
        Entity gameTitle;

        Button timeElapsedInfo;
        Button scoreInfo;
        Button highscoreInfo;
        TTF_Font* font;

};
