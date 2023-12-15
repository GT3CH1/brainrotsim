//
// Created by GT3CH1 on 11/13/23.
//

#ifndef PHYSICSSIM_BOX_HPP
#define PHYSICSSIM_BOX_HPP


#include "config.h"
#include "Color.h"
#include "renderer.h"

struct BoxInfo
{
    float x;
    float y;
    float w;
    float h;
    Color color;
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

    // ReSharper disable CppObjectMemberMightNotBeInitialized
    Box(b2World* world, SDL_Renderer* renderer)
    {
        float x = arc4random() % Config::SCREEN_CENTER_X * 2 + 1;
        float y = arc4random() % Config::SCREEN_CENTER_Y * 2 + 1;
        x = abs(x);
        y = abs(y);
        auto size = arc4random() % 5 + 1;

        init(world, renderer, x, y, size, size, Color::randColor());
        this->rect->w = size;
        this->rect->h = size;
        rect->x = body->GetPosition().x - rect->w / 2.0f;
        rect->y = body->GetPosition().y - rect->h / 2.0f;
    }

    Box(b2World* world, SDL_Renderer* renderer, float x, float y, float w, float h, Color color = Color::randColor())
    {
        init(world, renderer, x, y, w, h, color);
    }

    void init(b2World* world, SDL_Renderer* renderer, float x, float y, float w, float h, Color color)
    {
        this->color = color;
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set((float)x, (float)y);
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
        // set random angle
        //        body->SetAngularVelocity((float) (arc4random() % 10));
        int x_sign = arc4random() % 2 == 0 ? 1 : -1;
        int y_sign = arc4random() % 2 == 0 ? 1 : -1;

        body->ApplyForce(b2Vec2(x_sign * 1000 * body->GetMass(), y_sign * 1000 * body->GetMass()),
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

    void update(SDL_Renderer* the_renderer)
    {
        uint32 r = color.red();
        uint32 g = color.green();
        uint32 b = color.blue();

        SDL_SetRenderTarget(the_renderer, renderTexture);
        SDL_RenderClear(the_renderer);
        SDL_SetRenderDrawColor(the_renderer, r, g, b, 255);
        rect->x = body->GetPosition().x - rect->w / 2.0f;
        rect->y = body->GetPosition().y - rect->h / 2.0f;
        SDL_RenderFillRectF(the_renderer, rect);
        SDL_SetRenderTarget(the_renderer, render_target);
        SDL_RenderCopyF(the_renderer, renderTexture, nullptr, nullptr);
        num_audio_frames--;
    }

    static void onCollision(std::vector<BoxInfo*>* pending_boxes, Box* target)
    {
        if (arc4random() % 50 == 0)
        {
            const auto color = Color::randColor();
            const float x = fabs(arc4random() % 32);
            const float y = fabs(arc4random() % 24);
            pending_boxes->push_back(new BoxInfo{x, y, 1, 1, color});
        }

        if (Config::grow)
        {
            target->rect->w *= 1.01f;
            target->rect->h *= 1.01f;
        }
        if (Config::color_change_on_bounce)
            target->color = Color::randColor();
        const auto shape = dynamic_cast<b2PolygonShape*>(target->body->GetFixtureList()->GetShape());
        shape->SetAsBox(target->rect->w / 2.0f, target->rect->h / 2.0f);
        if (target->num_audio_frames <= 0)
        {
            target->num_audio_frames = 10;
            auto tone = 1760 / target->rect->w;
            if (tone < 27.50)
                tone = 27.50;
            target->tone = tone;
        }
    }
};

#endif //PHYSICSSIM_BOX_HPP
