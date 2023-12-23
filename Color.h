//
// Created by GT3CH1 on 11/20/23.
//

#ifndef PHYSICSSIM_COLOR_H
#define PHYSICSSIM_COLOR_H
#include <box2d/b2_types.h>
#include <cstdint>
#include <random>
#include <sys/types.h>

class Color {
public:
    uint32_t red() const { return (color >> 24) & 0xff; }

    uint32_t green() const { return (color >> 16) & 0xff; }

    uint32_t blue() const { return (color >> 8) & 0xff; }

    static Color randColor() {
        const auto r = rand() % 255;
        const auto g = rand() % 255;
        const auto b = rand() % 255;
        const auto color = 0x000000ff | (r << 24) | (g << 16) | (b << 8);
        return Color(color);
    }
    explicit Color(const uint32_t color) { this->color = color; }
    Color(const uint8 r, const uint8 g, const uint8 b) : Color(r, g, b, 255) {}

    Color(const uint8 r, const uint8 g, const uint8 b, const uint8 a) { color = (r << 24) | (g << 16) | (b << 8) | a; }
    Color() = default;
    Color(const Color &other) { color = other.color; }

    static Color fromInt(uint32_t color) { return Color(color); }
    static uint32_t toInt(const Color &color) { return color.color; }

private:
    uint32_t color;
};
static const Color WHITE = Color(255, 255, 255);
static const Color BLACK = Color(0, 0, 0);
#endif // PHYSICSSIM_COLOR_H
