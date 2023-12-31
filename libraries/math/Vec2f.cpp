//
// Created by GT3CH1 on 12/31/23.
//
#include "Vec2f.h"


#include <cmath>

float Vec2f::x() const { return _x; }

float Vec2f::y() const { return _y; }

Vec2f Vec2f::add(const Vec2f &other) const {
    const auto x = other.x() + this->_x;
    const auto y = this->_y + other.y();
    return Vec2f{x, y};
}

Vec2f Vec2f::subtract(const Vec2f &other) const {
    const auto x = this->_x - other.x();
    const auto y = this->_y - other.y();
    return Vec2f{x, y};
}

Vec2f Vec2f::divide(const Vec2f &other) const {
    const auto x = this->_x / other.x();
    const auto y = this->_y / other.x();
    return Vec2f{x, y};
}
Vec2f Vec2f::multiply(const Vec2f &other) const {
    const auto x = this->_x * other.x();
    const auto y = this->_y * other.y();
    return Vec2f{x, y};
}
float Vec2f::dot(const Vec2f &other) const {
    const float x = this->x() * other.x();
    const float y = this->y() * other.y();
    return x + y;
}

float Vec2f::length() const { return sqrt(x() * x() + y() * y()); }

float Vec2f::angle(const Vec2f &other) const {
    const auto a_norm = normalize();
    const auto b_norm = other.normalize();
    const float dot = a_norm.dot(b_norm);
    return acos(dot);
}

Vec2f Vec2f::normalize() const {
    const float _length = length();
    const float x = this->x() / _length;
    const float y = this->y() / _length;
    return Vec2f{x, y};
}


Vec2f::Vec2f(Vec2f &other) {
    this->_x = other.x();
    this->_y = other.y();
}
Vec2f::Vec2f(const Vec2f &other) {
    this->_x = other._x;
    this->_y = other._y;
}
