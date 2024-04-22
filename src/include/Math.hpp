#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ostream>

class Vector2f{
    public:
        float x = 0;
        float y = 0;
        Vector2f();
        Vector2f(float x, float p_y);

        Vector2f& add(const Vector2f& v);
        Vector2f& subtract(const Vector2f& v);
        Vector2f& multiply(const Vector2f& v);
        Vector2f& divide(const Vector2f& v);
    
        friend Vector2f operator+(const Vector2f& v1, const Vector2f& v2);
        friend Vector2f operator-(const Vector2f& v1, const Vector2f& v2);
        friend Vector2f operator*(const Vector2f& v1, const Vector2f& v2);
        friend Vector2f operator/(const Vector2f& v1, const Vector2f& v2);

        Vector2f& operator+=(const Vector2f& v);
        Vector2f& operator-=(const Vector2f& v);
        Vector2f& operator*=(const Vector2f& v);
        Vector2f& operator/=(const Vector2f& v);

        friend std::ostream& operator<<(std::ostream& stream, const Vector2f& v);
};
