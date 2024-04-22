#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>
#include <string>

#include "InputHandler.hpp"

#define MOVEMENT_UP 0
#define MOVEMENT_DOWN 1
#define MOVEMENT_LEFT 2
#define MOVEMENT_RIGHT 3

#define MOUSE_UP 0
#define MOUSE_DOWN 1

InputHandler::InputHandler(){
    /*
    const Uint8* keyboardStateList = SDL_GetKeyboardState(NULL);
    
    keyState[0] = !!keyboardStateList[SDL_SCANCODE_UP];
    keyState[1] = !!keyboardStateList[SDL_SCANCODE_DOWN];
    keyState[2] = !!keyboardStateList[SDL_SCANCODE_LEFT];
    keyState[3] = !!keyboardStateList[SDL_SCANCODE_RIGHT];

    */
}
void InputHandler::handleKeyboardEvent(const SDL_Event& event){
    bool keyState = (event.type == SDL_KEYDOWN);
    switch (event.key.keysym.sym){
        case SDLK_LEFT: // Left arrow key
            this->keyState[MOVEMENT_LEFT] = keyState;
            break;

        case SDLK_RIGHT: // Right arrow key
            this->keyState[MOVEMENT_RIGHT] = keyState;
            break;

        case SDLK_UP: // Up arrow key for jumping
            this->keyState[MOVEMENT_UP] = keyState;
            break;

    }
    //std::cout << this->keyState[MOVEMENT_LEFT] << " " << this->keyState[MOVEMENT_RIGHT] << " " << this->keyState[MOVEMENT_UP] << std::endl;
}

void InputHandler::handleMouseEvent(const SDL_Event& event){
    this->mouseState = (event.type == SDL_MOUSEBUTTONDOWN);
}

bool InputHandler::isPressed(SDL_Scancode p_scancode){
    if (p_scancode == SDL_SCANCODE_UP) return keyState[0] == SDL_PRESSED;
    if (p_scancode == SDL_SCANCODE_DOWN) return keyState[1] == SDL_PRESSED;
    if (p_scancode == SDL_SCANCODE_LEFT) return keyState[2] == SDL_PRESSED;
    if (p_scancode == SDL_SCANCODE_RIGHT) return keyState[3] == SDL_PRESSED;
    return SDL_RELEASED;
}

bool InputHandler::isReleased(SDL_Scancode p_scancode){
    if (p_scancode == SDL_SCANCODE_UP) return keyState[0] == SDL_RELEASED;
    if (p_scancode == SDL_SCANCODE_DOWN) return keyState[1] == SDL_RELEASED;
    if (p_scancode == SDL_SCANCODE_LEFT) return keyState[2] == SDL_RELEASED;
    if (p_scancode == SDL_SCANCODE_RIGHT) return keyState[3] == SDL_RELEASED;
    return SDL_PRESSED;
}

int InputHandler::getMouseState() const{
    return this->mouseState;
}

void InputHandler::updateEvent(const int& dir, const bool& value){
    this->keyState[dir] = value;
}