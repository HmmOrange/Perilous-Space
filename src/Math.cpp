#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ostream>

#include "Math.hpp"

Vector2f::Vector2f(){
    x = 0.0f;
    y = 0.0f;
}

Vector2f::Vector2f(float x, float y){
    this->x = x;
    this->y = y;
}

Vector2f& Vector2f::add(const Vector2f& v){
    this->x += v.x;
    this->y += v.y;

    return *this;
}

Vector2f& Vector2f::subtract(const Vector2f& v){
    this->x -= v.x;
    this->y -= v.y;

    return *this;
}


Vector2f& Vector2f::multiply(const Vector2f& v){
    this->x *= v.x;
    this->y *= v.y;

    return *this;
}


Vector2f& Vector2f::divide(const Vector2f& v){
    this->x /= v.x;
    this->y /= v.y;

    return *this;
}

Vector2f operator+(const Vector2f& v1, const Vector2f& v2){
    return Vector2f(v1.x + v2.x, v1.y + v2.y);
}

Vector2f operator-(const Vector2f& v1, const Vector2f& v2){
    return Vector2f(v1.x - v2.x, v1.y - v2.y);
}

Vector2f operator*(const Vector2f& v1, const Vector2f& v2){
    return Vector2f(v1.x * v2.x, v1.y * v2.y);
}

Vector2f operator/(const Vector2f& v1, const Vector2f& v2){
    return Vector2f(v1.x / v2.x, v1.y / v2.y);
}

Vector2f& Vector2f::operator+=(const Vector2f& v){
    return this->add(v);
}

Vector2f& Vector2f::operator-=(const Vector2f& v){
    return this->subtract(v);
}

Vector2f& Vector2f::operator*=(const Vector2f& v){
    return this->multiply(v);
}

Vector2f& Vector2f::operator/=(const Vector2f& v){
    return this->divide(v);
}

std::ostream& operator<<(std::ostream& stream, const Vector2f& v){
    stream << "(" << v.x << ", " << v.y  << ")";
    return stream;
}
