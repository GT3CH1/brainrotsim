//
// Created by GT3CH1 on 11/13/23.
//

#ifndef PHYSICSSIM_BOX_HPP
#define PHYSICSSIM_BOX_HPP


#include "consts.h"
#include "config.h"
#include "Color.h"

struct BoxInfo {
public:
    float x;
    float y;
    float w;
    float h;
    Color color;
};

class Box {
public:
    Color color;

    SDL_FRect *rect;
    b2Body *body;

    SDL_Texture *renderTexture;

    int num_audio_frames = 0;

    Box(b2World *world, SDL_Renderer *renderer) {
        auto x = (float) (arc4random() % (int) Config::SCREEN_CENTER_X * 2 + 1);
        auto y = (float) (arc4random() % (int) Config::SCREEN_CENTER_Y * 2 + 1);
        x = abs(x);
        y = abs(y);
        init(world, renderer, x, y, 1, 1, Color::randColor());
        this->rect->w = arc4random() % 20 + 1;
        this->rect->h = this->rect->w;
    }

    Box(b2World *world, SDL_Renderer *renderer, float x, float y, float w, float h, Color color = Color::randColor()) {
        init(world, renderer, x, y, w, h, color);
    }

    void init(b2World *world, SDL_Renderer *renderer, float x, float y, float w, float h, Color color) {
        this->color = color;
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set((float) x, (float) y);
        bodyDef.gravityScale = 0.0f;
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
        // set random angle
//        body->SetAngularVelocity((float) (arc4random() % 10));
        int x_sign = arc4random() % 2 == 0 ? 1 : -1;
        int y_sign = arc4random() % 2 == 0 ? 1 : -1;

        body->ApplyForce(b2Vec2(x_sign * 10000 * body->GetMass(), y_sign * 10000 * body->GetMass()),
                         body->GetWorldCenter(),
                         true);
        body->SetTransform(body->GetPosition(), 0);
        rect = new SDL_FRect();
        rect->x = x - w / 2.0f;
        rect->y = y - h / 2.0f;
        rect->w = w;
        rect->h = h;
        renderTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w, h);

    }

    void update(SDL_Renderer *renderer) {
        uint32 r = color.red();
        uint32 g = color.green();
        uint32 b = color.blue();
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        rect->x = body->GetPosition().x - rect->w / 2.0f;
        rect->y = body->GetPosition().y - rect->h / 2.0f;

        SDL_SetRenderTarget(renderer, renderTexture);
        SDL_RenderClear(renderer);

        r = color.red();
        g = color.green();
        b = color.blue();
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderFillRectF(renderer, rect);
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderCopyExF(renderer, renderTexture, nullptr, rect, body->GetAngle() * 180 / M_PI, nullptr,
                          SDL_FLIP_NONE);
        num_audio_frames--;

    }

    static void onCollision(std::vector<BoxInfo *> *pending_boxes) {
        if (arc4random() % 50 == 0) {
            Color color = Color::randColor();
            float x = fabs(arc4random() % (int) 32);
            float y = fabs(arc4random() % (int) 24);
            pending_boxes->push_back(new BoxInfo{x, y, 1, 1, color});
        }
    }


};

#endif //PHYSICSSIM_BOX_HPP
