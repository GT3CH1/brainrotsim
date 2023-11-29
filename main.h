//
// Created by GT3CH1 on 11/20/23.
//

#ifndef PHYSICSSIM_MAIN_H
#define PHYSICSSIM_MAIN_H

#include "Wall.h"
#include "box.hpp"
#include "consts.h"
#include "Keyboard.hpp"
#include <SDL2/SDL.h>
#include <vector>

class Main {
public:
    static b2World *world;
    static SDL_Renderer *renderer;
    static SDL_Window *window;

    static Wall *bottom;
    static Wall *top;
    static Wall *left;
    static Wall *right;

    static std::vector<Box *> boxes;
    static std::vector<BoxInfo *> *pending_boxes;

    static void setup();

    static void run();

    static SDL_Rect bg;
    
    static int sound_idx;

    static void setScreenDimensions(bool fullscreen = false);
};

#endif //PHYSICSSIM_MAIN_H
