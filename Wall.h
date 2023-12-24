//
// Created by GT3CH1 on 11/20/23.
//

#ifndef PHYSICSSIM_WALL_H
#define PHYSICSSIM_WALL_H

#include <SDL2/SDL_rect.h>
#include <box2d/box2d.h>
static int wall_counter = 0;
class Wall {
public:
    SDL_FRect rect;
    b2Body *body;
    int uuid;
    Wall(const float x, const float y, const float w, const float h, b2World *world) {
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(x, y);
        b2PolygonShape groundBox;
        groundBox.SetAsBox(w, h);
        b2Body *groundBody = world->CreateBody(&groundBodyDef);
        groundBody->CreateFixture(&groundBox, 0.0f);
        body = groundBody;
        rect = SDL_FRect();
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;
        uuid = wall_counter;
        wall_counter++;
    }
};

#endif // PHYSICSSIM_WALL_H
