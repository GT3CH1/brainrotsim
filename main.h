//
// Created by GT3CH1 on 11/20/23.
//

#ifndef PHYSICSSIM_MAIN_H
#define PHYSICSSIM_MAIN_H

#include "Wall.h"
#include "box.hpp"
#include "beeper.h"
#include <SDL2/SDL.h>
#include <vector>
#include "renderer.h"

class Main
{
public:
    static b2World* world;
    static SDL_Renderer* renderer;
    static SDL_Window* window;
    static Wall* bottom;
    static Wall* top;
    static Wall* left;
    static Wall* right;
    static TTF_Font* font;

    static std::vector<Box*> boxes;
    static std::vector<BoxInfo*>* pending_boxes;

    static void setup();

    static void run();

    static SDL_Rect bg;

    static int sound_idx;
    static int devId;
    static void setScreenDimensions(bool fullscreen = false);
    static void handle_keyboard();
    static void handle_key_event();
    static Beeper beeper;
};

b2World* Main::world = nullptr;
SDL_Renderer* Main::renderer;
SDL_Window* Main::window;
Beeper Main::beeper;
Wall* Main::bottom = nullptr;
Wall* Main::top = nullptr;
Wall* Main::left = nullptr;
Wall* Main::right = nullptr;
std::vector<Box*> Main::boxes;
std::vector<BoxInfo*>* Main::pending_boxes = new std::vector<BoxInfo*>();
SDL_Rect Main::bg;
TTF_Font* Main::font = nullptr;


class ContactListener final : public b2ContactListener
{
    void BeginContact(b2Contact* contact) override
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();
        const auto boxA = reinterpret_cast<Box*>(fixtureA->GetUserData().pointer);
        const auto boxB = reinterpret_cast<Box*>(fixtureB->GetUserData().pointer);
        if (boxA && boxB && !Config::collision)
            return;
        if (boxA)
        {
            Box::onCollision(Main::pending_boxes, boxA);
        }
        if (boxB)
        {
            Box::onCollision(Main::pending_boxes, boxB);
        }
    }

    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
    override
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();
        const Box* boxA = reinterpret_cast<Box*>(fixtureA->GetUserData().pointer);
        const Box* boxB = reinterpret_cast<Box*>(fixtureB->GetUserData().pointer);
        if (boxA && boxB && !Config::collision)
            contact->SetEnabled(false);
    }
};


#endif //PHYSICSSIM_MAIN_H
