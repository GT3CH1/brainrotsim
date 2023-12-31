//
// Created by GT3CH1 on 11/13/23.
//

#ifndef PHYSICSSIM_BOX_HPP
#define PHYSICSSIM_BOX_HPP


#include <box2d/box2d.h>
#include <vector>
#include "Color.h"
#include "Entity.h"
#include "simrenderer.h"

struct BoxInfo {
    Vec2f pos;
    float w;
    float h;
    uint32_t color;
};

class Box : public Entity {
public:
    int num_audio_frames = 0;
    float tone = 0;
    explicit Box(b2World *world);
    Box(b2World *world, const Vec2f &pos, float w, float h, const Color &color);
    ~Box();
    void init(b2World *world, const Vec2f &pos, float w, float h, const Color &color);
    void update(SDL_Renderer *the_renderer);
    /**
     * \brief Draws an outline around the box.
     */
    void drawOutline() const;

    void setColor(const Color &color);

    float getWindowX() const;
    float getWindowY() const;
    float getScreenWidth() const;
    float getRectSize() const;

    Color getColor() const;

    SDL_FRect getScreenRect() const;
    std::vector<SDL_FPoint> *collision_locs;
    static void onCollision(std::vector<BoxInfo *> *pending_boxes, Box *target);
    static void handleWireFrameToggle();
    static inline int BOX_SPAWN_PROBABILITY = 50;

private:
    SDL_FRect *mFRect;
};

#endif // PHYSICSSIM_BOX_HPP
