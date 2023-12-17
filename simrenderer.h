//
// Created by GT3CH1 on 12/15/23.
//

#ifndef PHYSICSSIM_RENDERER_H
#define PHYSICSSIM_RENDERER_H

#include <SDL2/SDL_render.h>

class Renderer
{
public:
    static SDL_Renderer* renderer;
    static SDL_Surface* render_surface;
    static SDL_Texture* render_target;
};

#endif //PHYSICSSIM_RENDERER_H
