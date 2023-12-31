//
// Created by GT3CH1 on 12/31/23.
//

#include "Entity.h"
#include <random>

#include "config.h"
Vec2f Entity::getRandCoords() {
    float x = random() % (int) Config::SCREEN_CENTER_X * 2 + 1;
    float y = random() % (int) Config::SCREEN_CENTER_Y * 2 + 1;
    x = abs(x);
    y = abs(y);
    return Vec2f{x, y};
}
b2Vec2 Entity::bodyPos() const { return getBody()->GetPosition(); }
b2Body *Entity::getBody() const { return mBody; }
