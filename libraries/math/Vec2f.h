//
// Created by GT3CH1 on 12/31/23.
//

#ifndef VEC2D_H
#define VEC2D_H


struct Vec2f {
    Vec2f(const float x, const float y) : _x(x), _y(y) {}
    Vec2f(Vec2f &other);
    Vec2f(const Vec2f &other);
    [[nodiscard]] float x() const;
    [[nodiscard]] float y() const;

    [[nodiscard]] Vec2f add(const Vec2f &other) const;
    [[nodiscard]] Vec2f subtract(const Vec2f &other) const;
    [[nodiscard]] Vec2f divide(const Vec2f &other) const;
    [[nodiscard]] Vec2f multiply(const Vec2f &other) const;
    [[nodiscard]] Vec2f normalize() const;
    [[nodiscard]] float dot(const Vec2f &other) const;
    [[nodiscard]] float angle(const Vec2f &other) const;
    [[nodiscard]] float length() const;

private:
    float _x;
    float _y;
};

#endif // VEC2D_H
