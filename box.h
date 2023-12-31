//
// Created by GT3CH1 on 11/13/23.
//

#ifndef PHYSICSSIM_BOX_HPP
#define PHYSICSSIM_BOX_HPP


#include <box2d/box2d.h>
#include <vector>
#include "Color.h"
#include "simrenderer.h"

struct BoxInfo {
    float x;
    float y;
    float w;
    float h;
    uint32_t color;
};

class Box {
public:
    Color color;
    SDL_FRect *rect;
    b2Body *body;
    SDL_Texture *renderTexture;
    int num_audio_frames = 0;
    float tone = 0;
    explicit Box(b2World *world);
    Box(b2World *world, float x, float y, float w, float h, const Color &color);
    ~Box();
    void init(b2World *world, float x, float y, float w, float h, const Color &color);
    void update(SDL_Renderer *the_renderer);
    static void onCollision(std::vector<BoxInfo *> *pending_boxes, Box *target);
    float getWindowX() const;
    float getWindowY() const;
    float getScreenWidth() const;
    SDL_FRect getScreenRect() const;
    static void handleWireFrameToggle();
    std::vector<SDL_FPoint> *collision_locs;
    static inline int BOX_SPAWN_PROBABILITY = 50;
};

#endif // PHYSICSSIM_BOX_HPP
