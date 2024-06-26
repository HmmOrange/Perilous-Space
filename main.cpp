#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "WindowRenderer.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Math.hpp"
#include "InputHandler.hpp"
#include "Game.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define CURSOR_IMAGE_PATH "./assets/images/cursor.png"
#define CLICK_SFX_PATH "./assets/audio/click.wav"

const float FIXED_FRAME_RATE = 1.0/60;

int main(int argc, char *argv[]){
    
    SDL_Init(SDL_INIT_EVERYTHING);
    
    WindowRenderer renderer("Square Scramble :D", SCREEN_WIDTH, SCREEN_HEIGHT);
    int refreshRate = renderer.getRefreshRate();

    /* Initializing Game */
    SDL_Event event;
    InputHandler inputHandler;

    SDL_Surface *cursorSurface = IMG_Load(CURSOR_IMAGE_PATH);
    SDL_Cursor *cursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
    SDL_SetCursor(cursor);

    // Do not use () for arg-less class.
    Game game(renderer);
    //game.startNewGame(renderer);


    Mix_Chunk* clickSFX = Mix_LoadWAV(CLICK_SFX_PATH);
    if (!clickSFX) {
        std::cout << "Failed to load click WAV file: " << Mix_GetError() << std::endl;
    }
    
    Uint32 prevTime = SDL_GetTicks();
    float frameCounter = 0;
    float unprocessedTime = 0;
    int frames = 0;
    bool do_render = false;

    while (game.isRunning()){
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - prevTime); // Convert milliseconds to seconds
        if (deltaTime > FIXED_FRAME_RATE) deltaTime = FIXED_FRAME_RATE;
        prevTime = currentTime;
        do_render = false;
        
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                game.updateState(GAME_QUIT);
                break;
            }

            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){
                inputHandler.handleKeyboardEvent(event);
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP){
                inputHandler.handleMouseEvent(event);
            }
        }

        // Fixing time step and FPS counter
        unprocessedTime += deltaTime;
        frameCounter += deltaTime;
        if (frameCounter >= 1.0){
            //std::cout << "FPS: " << frames << " | " << 1000 / frames << "ms" << std::endl;
            frames = 0;
            frameCounter = 0;
        }

        while (unprocessedTime > FIXED_FRAME_RATE){
            do_render = true;
            unprocessedTime -= FIXED_FRAME_RATE;
        }

        int currentGameState = game.getState();
        
        if (currentGameState == GAME_MAIN_MENU){
            game.renderMainMenu(renderer);

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            game.getMainMenu()->updateButtonState(mouseX, mouseY, inputHandler.getMouseState());
            if (game.hasClickedStart()){
                Mix_PlayChannel(-1, clickSFX, 0);
                game.startNewGame(renderer);
            }
            else if (game.hasClickedQuit()){
                game.updateState(GAME_QUIT);
            }
        }
        else if (currentGameState == GAME_PLAYING){
            game.processGameEvents(renderer, inputHandler, deltaTime);
            if (do_render){
                game.renderPlaying(renderer);
                frames++;
            }
            else SDL_Delay(1);
        }
        else if (currentGameState == GAME_END){
            bool rendered = false;
            if (!rendered) game.renderMainMenu(renderer);
            rendered = true;

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            game.getMainMenu()->updateButtonState(mouseX, mouseY, inputHandler.getMouseState());
            if (game.hasClickedStart()){
                game.startNewGame(renderer);
                Mix_PlayChannel(-1, clickSFX, 0);
            }
            else if (game.hasClickedQuit())
                game.updateState(GAME_QUIT);
        }
        
    }
    
    renderer.cleanUp();
    SDL_Quit();
    return 0;
}

//mingw32-make