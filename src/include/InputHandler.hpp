#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>
#include <string>

#define MOVEMENT_UP 0
#define MOVEMENT_DOWN 1
#define MOVEMENT_LEFT 2
#define MOVEMENT_RIGHT 3

class InputHandler{
    public:
        InputHandler();
        void handleKeyboardEvent(const SDL_Event& event);
        void handleMouseEvent(const SDL_Event& event);
        bool isPressed(SDL_Scancode p_scancode);
        bool isReleased(SDL_Scancode p_scancode);
        int getMouseState() const;

        bool keyState[4] = {0};
        /*
            UP: 0
            DOWN: 1
            LEFT: 2
            RIGHT: 3
        */
        int mouseState = 0;
        void updateEvent(const int& dir, const bool& value);
};
