//
// Created by GT3CH1 on 12/17/23.
// Copyright (c) 2023 GT3CH1. All rights reserved.

#include "box.h"

Box::Box(b2World *world) {
    rect = nullptr;
    renderTexture = nullptr;
    body = nullptr;
    float x = arc4random() % Config::SCREEN_CENTER_X * 2 + 1;
    float y = arc4random() % Config::SCREEN_CENTER_Y * 2 + 1;
    x = abs(x);
    y = abs(y);
    const auto size = arc4random() % 5 + 1;

    init(world, x, y, size, size, Color::randColor());
    this->rect->w = size;
    this->rect->h = size;
    rect->x = body->GetPosition().x - rect->w / 2.0f;
    rect->y = body->GetPosition().y - rect->h / 2.0f;
}

/**
 * \brief
 * \param world The box2d world pointer.
 * \param x The current X coordinate of the box.
 * \param y The current Y coordinate of the box.
 * \param w The width of the box.
 * \param h The height of the box.
 * \param color The color of the box.
 */
Box::Box(b2World *world, const float x, const float y, const float w, const float h, const Color &color) {
    rect = nullptr;
    renderTexture = nullptr;
    body = nullptr;
    init(world, x, y, w, h, color);
}

void Box::init(b2World *world, const float x, const float y, const float w, const float h, const Color &color) {
    this->color = color;
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    // bodyDef.gravityScale = 0.0f;
    bodyDef.fixedRotation = true;
    body = world->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((w / 2.0f), (h / 2.0f));
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 1.0f;
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    body->CreateFixture(&fixtureDef);

    const int x_sign = arc4random() % 2 == 0 ? 1 : -1;
    const int y_sign = arc4random() % 2 == 0 ? 1 : -1;

    body->ApplyForce(b2Vec2(x_sign * 1000 * body->GetMass(), y_sign * 1000 * body->GetMass()), body->GetWorldCenter(),
                     true);
    body->SetTransform(body->GetPosition(), 0);
    rect = new SDL_FRect();
    rect->x = x - w / 2.0f;
    rect->y = y - h / 2.0f;
    rect->w = w;
    rect->h = h;
}

void Box::update(SDL_Renderer *the_renderer) {
    Renderer::setDrawColor(color);
    rect->x = body->GetPosition().x - rect->w / 2.0f;
    rect->y = body->GetPosition().y - rect->h / 2.0f;
    SDL_RenderFillRectF(the_renderer, rect);
    num_audio_frames--;
}

void Box::onCollision(std::vector<BoxInfo *> *pending_boxes, Box *target) {
    if (arc4random() % 50 == 0) {
        Color color = Color::randColor();
        const float x = fabs(arc4random() % 32);
        const float y = fabs(arc4random() % 24);
        pending_boxes->push_back(new BoxInfo{x, y, 1, 1, color});
    }

    if (Config::grow) {
        target->rect->w *= 1.01f;
        target->rect->h *= 1.01f;
    }
    if (Config::color_change_on_bounce)
        target->color = Color::randColor();
    const auto shape = dynamic_cast<b2PolygonShape *>(target->body->GetFixtureList()->GetShape());
    shape->SetAsBox(target->rect->w / 2.0f, target->rect->h / 2.0f);
    if (target->num_audio_frames <= 0) {
        target->num_audio_frames = 10;
        auto tone = 1760 / target->rect->w;
        if (tone < 27.50)
            tone = 27.50;
        target->tone = tone;
    }
}
