//
// Created by GT3CH1 on 12/17/23.
// Copyright (c) 2023 GT3CH1. All rights reserved.

#include "box.h"


#include "Layer.h"
#include "LineModes.h"
#include "blendmodes.h"

Box::Box(b2World *world) {
    mFRect = nullptr;
    mBody = nullptr;

    const auto size = rand() % 5 + 1;
    init(world, getRandCoords(), size, size, Color::randColor());
    this->mFRect->w = size;
    this->mFRect->h = size;
    mFRect->x = mBody->GetPosition().x - mFRect->w / 2.0f;
    mFRect->y = mBody->GetPosition().y - mFRect->h / 2.0f;
}

/**
 * \brief
 * \param world The box2d world pointer.
 * \param pos Current position.
 * \param w The width of the box.
 * \param h The height of the box.
 * \param color The color of the box.
 */
Box::Box(b2World *world, const Vec2f &pos, const float w, const float h, const Color &color) {
    mFRect = nullptr;
    mBody = nullptr;
    init(world, pos, w, h, color);
}

Box::~Box() { delete collision_locs; }

void Box::init(b2World *world, const Vec2f &pos, const float w, const float h, const Color &color) {
    this->mColor = color;
    b2BodyDef mBodyDef;
    mBodyDef.type = b2_dynamicBody;
    mBodyDef.position.Set(pos.x(), pos.y());
    // mBodyDef.gravityScale = 0.0f;
    mBodyDef.fixedRotation = true;
    mBody = world->CreateBody(&mBodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((w / 2.0f), (h / 2.0f));
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 1.0f;
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    mBody->CreateFixture(&fixtureDef);

    const int x_sign = rand() % 2 == 0 ? 1 : -1;
    const int y_sign = rand() % 2 == 0 ? 1 : -1;

    mBody->ApplyForce(b2Vec2(x_sign * 1000 * mBody->GetMass(), y_sign * 1000 * mBody->GetMass()),
                      mBody->GetWorldCenter(), true);
    mBody->SetTransform(mBody->GetPosition(), 0);
    mFRect = new SDL_FRect();
    mFRect->x = pos.x() - w / 2.0f;
    mFRect->y = pos.y() - h / 2.0f;
    mFRect->w = w;
    mFRect->h = h;

    collision_locs = new std::vector<SDL_FPoint>();
}

void Box::update(SDL_Renderer *the_renderer) {
    Renderer::setDrawColor(mColor);
    mFRect->x = mBody->GetPosition().x - mFRect->w / 2.0f;
    mFRect->y = mBody->GetPosition().y - mFRect->h / 2.0f;
    SDL_RenderFillRectF(the_renderer, mFRect);
    num_audio_frames--;
}

void Box::onCollision(std::vector<BoxInfo *> *pending_boxes, Box *target) {
    if (rand() % BOX_SPAWN_PROBABILITY == 0) {
        Color color = Color::randColor();
        float x = abs(rand() % (int) Config::SCREEN_CENTER_X * 2 + 1);
        float y = abs(rand() % (int) Config::SCREEN_CENTER_Y * 2 + 1);
        pending_boxes->push_back(new BoxInfo{{x, y}, 1, 1, Color::toInt(color)});
    }

    if (Config::grow) {
        target->mFRect->w *= 1.01f;
        target->mFRect->h *= 1.01f;
    }
    if (Config::color_change_on_bounce)
        target->mColor = Color::randColor();
    const auto shape = dynamic_cast<b2PolygonShape *>(target->mBody->GetFixtureList()->GetShape());
    shape->SetAsBox(target->mFRect->w / 2.0f, target->mFRect->h / 2.0f);
    if (target->num_audio_frames <= 0) {
        target->num_audio_frames = 10;
        auto tone = 1760 / target->mFRect->w;
        if (tone < 27.50)
            tone = 27.50;
        target->tone = tone;
    }
}

float Box::getWindowX() const { return (mBody->GetPosition().x - mFRect->w / 2.0f) * Renderer::WINDOW_SCALE; }

float Box::getWindowY() const { return (mBody->GetPosition().y - mFRect->h / 2.0f) * Renderer::WINDOW_SCALE; }

float Box::getScreenWidth() const { return mFRect->w * Renderer::WINDOW_SCALE; }
float Box::getRectSize() const { return this->mFRect->w; }
Color Box::getColor() const { return mColor; }

SDL_FRect Box::getScreenRect() const { return {getWindowX(), getWindowY(), getScreenWidth(), getScreenWidth()}; }

void Box::drawOutline() const {
    auto const _b = getScreenRect();
    Renderer::drawRect(&_b, false);
}
void Box::setColor(const Color &color) { this->mColor = color; }


void Box::handleWireFrameToggle() {
    Config::wireframe = !Config::wireframe;
    Config::outline = Config::wireframe;
    Config::line_mode = Config::wireframe;
    Renderer::clearLayer(Layer::OUTLINE);
    Renderer::clearLayer(Layer::LINE);
    if (Config::wireframe) {
        Config::smear_line = true;
        Config::outline = true;
        Config::line_mode = true;
        if (Config::current_linemode == LineMode::NONE)
            Config::current_linemode = LineMode::CENTER;
    } else {
        Config::smear_line = false;
    }
    SDL_BlendMode _customBlend;
    if (Config::wireframe) {
        _customBlend = wireframe_blend_mode;
    } else {
        _customBlend = normal_blend_mode;
    }
    Renderer::setBlendMode(Layer::LINE, SDL_BLENDMODE_NONE);
    Renderer::setBlendMode(Layer::BOX, _customBlend);
    Renderer::setBlendMode(Layer::OUTLINE, _customBlend);
}
