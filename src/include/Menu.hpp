#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Button.hpp"
#include "Math.hpp"

class Menu{
    public: 
        Menu();
        void addButton(Button& button);
        void updateButtonState(int mouseX, int mouseY, int mouseState);
        std::vector<Button> getButtonList() const;

    private:
        std::vector<Button> buttonList;
};
