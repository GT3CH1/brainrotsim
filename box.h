//
// Created by GT3CH1 on 11/13/23.
//

#ifndef PHYSICSSIM_BOX_HPP
#define PHYSICSSIM_BOX_HPP


#include "Color.h"
#include <vector>
#include <box2d/box2d.h>
#include "config.h"
#include "simrenderer.h"

struct BoxInfo
{
    float x;
    float y;
    float w;
    float h;
    Color& color;
};

class Box
{
public:
    Color color;
    SDL_FRect* rect;
    b2Body* body;
    SDL_Texture* renderTexture;
    int num_audio_frames = 0;
    float tone = 0;
    Box(b2World* world, SDL_Renderer* renderer);
    Box(b2World* world, SDL_Renderer* renderer, float x, float y, float w, float h, const Color& color);
    void init(b2World* world, SDL_Renderer* renderer, float x, float y, float w, float h,
              const Color& color);
    void update(SDL_Renderer* the_renderer);
    static void onCollision(std::vector<BoxInfo*>* pending_boxes, Box* target);
};

#endif //PHYSICSSIM_BOX_HPP
