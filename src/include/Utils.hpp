#pragma once

#include <SDL2/SDL.h>
#include <SDl2/SDL_image.h>
#include <chrono>
#include <random>

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

namespace utils{
    inline double getTimeInSeconds(){
        double tick = SDL_GetTicks();
        tick *= 0.001;
        return tick;
    }

    inline int random(int l, int r){
        return rng() % (r - l + 1) + l;
    }

    void drawPie(WindowRenderer renderer, int centerX, int centerY, int radius, double startAngle, double endAngle) {
        SDL_SetRenderDrawColor(renderer.getRenderer(), 118, 2, 137, 255); // Purple color
        
        double startRad = (startAngle - 90) * M_PI / 180.0;
        double endRad = (endAngle - 90) * M_PI / 180.0;

        if (endRad < startRad) {
            endRad += 2 * M_PI;
        }

        double angleStep = 0.01; 
        int thickness = 4;

        std::vector<SDL_Point> points;
        for (int t = 0; t < thickness; t++) {
            double currentRadius = radius + t;
            for (double angle = startRad; angle <= endRad; angle += angleStep) {
                int x = centerX + currentRadius * cos(angle);
                int y = centerY + currentRadius * sin(angle);
                points.push_back({x, y});
            }

            // Ensure the last point is drawn
            int x = centerX + currentRadius * cos(endRad);
            int y = centerY + currentRadius * sin(endRad);
            points.push_back({x, y});
        }

        SDL_RenderDrawLines(renderer.getRenderer(), points.data(), points.size());

        SDL_SetRenderDrawColor(renderer.getRenderer(), 118, 2, 137, 0); // Purple color
    }
}   