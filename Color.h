//
// Created by GT3CH1 on 11/20/23.
//

#ifndef PHYSICSSIM_COLOR_H
#define PHYSICSSIM_COLOR_H
#include <cstdint>
#include <random>

class Color
{
public:
    uint32_t red() const
    {
        return (color >> 24) & 0xff;
    }

    uint32_t green() const
    {
        return (color >> 16) & 0xff;
    }

    uint32_t blue() const
    {
        return (color >> 8) & 0xff;
    }

    static Color randColor()
    {
        auto r = arc4random() % 255;
        auto g = arc4random() % 255;
        auto b = arc4random() % 255;
        auto color = 0x000000ff | (r << 24) | (g << 16) | (b << 8);
        Color c = Color();
        c.color = color;
        return c;
    }

private:
    uint32_t color;
};

#endif //PHYSICSSIM_COLOR_H
