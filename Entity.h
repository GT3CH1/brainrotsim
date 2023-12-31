//
// Created by GT3CH1 on 12/31/23.
//

#ifndef ENTITY_H
#define ENTITY_H
#include <SDL2/SDL_rect.h>


#include <box2d/box2d.h>
#include "Color.h"
#include "math/Vec2f.h"

class Entity {
public:
    static Vec2f getRandCoords();
    Entity() : mPos(Vec2f(0, 0)) {}
    virtual void update(){};

    b2Vec2 bodyPos() const;
    b2Body *getBody() const;

protected:
    Color mColor;
    b2Body *mBody;
    Vec2f mPos;
};


#endif // ENTITY_H
