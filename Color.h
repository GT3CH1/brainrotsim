//
// Created by GT3CH1 on 11/20/23.
//

#ifndef PHYSICSSIM_COLOR_H
#define PHYSICSSIM_COLOR_H

class Color {

public:

    uint32 red() const {
        return (color >> 24) & 0xff;
    }

    uint32 green() const {
        return (color >> 16) & 0xff;
    }

    uint32 blue() const {
        return (color >> 8) & 0xff;
    }

    static Color randColor() {
        uint32 r = random() % 255;
        uint32 g = random() % 255;
        uint32 b = random() % 255;
        uint32 color = 0x000000ff | (r << 24) | (g << 16) | (b << 8);
        Color c = Color();
        c.color = color;
        return c;
    }

private:
    uint32 color;
};

#endif //PHYSICSSIM_COLOR_H
