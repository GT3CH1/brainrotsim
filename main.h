//
// Created by GT3CH1 on 11/20/23.
//

#ifndef PHYSICSSIM_MAIN_H
#define PHYSICSSIM_MAIN_H

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "Keyboard.hpp"
#include "LineModes.h"
#include "Wall.h"
#include "beeper.h"
#include "blendmodes.h"
#include "box.h"
#include "config.h"
#include "simrenderer.h"
/**
 * \brief Contains information and methods for handling the simulation.
 */
class Main {
public:
    /**
     * \brief The box2d world.
     */
    static b2World *world;
    /**
     * \brief Bottom wall.
     */
    static Wall *bottom;
    /**
     * \brief Top wall.
     */
    static Wall *top;
    /**
     * \brief Left wall.
     */
    static Wall *left;
    /**
     * \brief Right wall.
     */
    static Wall *right;
    /**
     * \brief TODO: font.
     */
    // static TTF_Font *font;
    /**
     * \brief The list of boxes.
     */
    static std::vector<Box *> boxes;
    /**
     * \brief The list of pending boxes to be added.
     */
    static std::vector<BoxInfo *> *pending_boxes;

    /**
     * \brief Sets up the simulation.
     */
    static void setup();

    /**
     * \brief Main simulation loop.
     */
    static void run();

    /**
     * \brief The gravity of the simulation.
     */
    inline static float GRAVITY = 0;
    /**
     * \brief Sets up the box2d world.
     */
    static void setupWorld();
    /**
     * \brief Resets the world, and the renderer.
     */
    static void reset_simulation();

    static void spawnBox();
    /**
     * \brief Handles keybindings.
     */
    static void handleKeybind();

    /**
     * \brief Handles key press events from SDL.
     */
    static void onKeyPress();

    /**
     * \brief Draws a line from the center of the screen to the center of the given box.
     * \param box The box to draw a line to.
     */
    static void drawLineToBox(const Box *box);

    /**
     * \brief Adds pending boxes to the main box list.
     */
    static void addNewBoxes();

    static void handleLineModes();
    /**
     * \brief Renders all boxes in the simulation.
     * \param box_has_audio Whether to play an audio tone.
     */
    static void render_boxes(bool &box_has_audio);

    /**
     * \brief The beeper/tone generator.
     */
    static Beeper beeper;

    /**
     * \brief The number of velocity iterations for box2d.
     */
    static inline int NUM_VEL_ITERATIONS = 1;

    /**
     * \brief The number of position iterations for box2d.
     */
    static inline int NUM_POS_ITERATIONS = 1;

    /**
     * \brief The delta time change for box2d (1/60).
     */
    static inline float WORLD_STEP = 0.01666f;

    static std::vector<Wall *> *walls;

    static bool foundWall(Wall target) {
        for (const auto wall: *walls) {
            if (wall->uuid == target.uuid)
                return true;
        }
        return false;
    }
};

b2World *Main::world = nullptr;
Beeper Main::beeper;
std::vector<Wall *> *Main::walls = new std::vector<Wall *>();
Wall *Main::bottom = nullptr;
Wall *Main::top = nullptr;
Wall *Main::left = nullptr;
Wall *Main::right = nullptr;
std::vector<Box *> Main::boxes;
std::vector<BoxInfo *> *Main::pending_boxes = new std::vector<BoxInfo *>();


/**
 * \brief The contact listener for box2d. Handles box collision and events.
 */
class ContactListener final : public b2ContactListener {
    void BeginContact(b2Contact *contact) override {
        b2Fixture *fixtureA = contact->GetFixtureA();
        b2Fixture *fixtureB = contact->GetFixtureB();
        const auto boxA = reinterpret_cast<Box *>(fixtureA->GetUserData().pointer);
        const auto boxB = reinterpret_cast<Box *>(fixtureB->GetUserData().pointer);
        if (boxA && boxB && !Config::collision)
            return;
        if (boxA)
            Box::onCollision(Main::pending_boxes, boxA);
        if (boxB)
            Box::onCollision(Main::pending_boxes, boxB);
    }

    static SDL_FPoint getCollisionPoint(const Box *boxA) {
        SDL_FPoint pt{boxA->getWindowX(), boxA->getWindowY()};
        if (pt.x < 1.5)
            pt.x = 0;
        else if (boxA->getWindowX() + (boxA->getRectSize() * 10) >
                 Config::SCREEN_WIDTH - Config::SCREEN_WIDTH * 0.02f) {
            pt.x = Config::SCREEN_WIDTH;
        } else
            pt.x = boxA->bodyPos().x * Renderer::WINDOW_SCALE;
        if (pt.y < 1.5)
            pt.y = 0;
        else if (boxA->getWindowY() + (boxA->getRectSize() * 10) >
                 Config::SCREEN_HEIGHT - Config::SCREEN_HEIGHT * 0.02f) {
            pt.y = Config::SCREEN_HEIGHT;
        } else
            pt.y = boxA->bodyPos().y * Renderer::WINDOW_SCALE;
        return pt;
    }
    void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) override {
        b2Fixture *fixtureA = contact->GetFixtureA();
        b2Fixture *fixtureB = contact->GetFixtureB();
        const auto *boxA = reinterpret_cast<Box *>(fixtureA->GetUserData().pointer);
        const auto *boxB = reinterpret_cast<Box *>(fixtureB->GetUserData().pointer);
        const auto *wallA = reinterpret_cast<Wall *>(fixtureA->GetUserData().pointer);
        const auto *wallB = reinterpret_cast<Wall *>(fixtureB->GetUserData().pointer);
        bool wallAFound = false;
        bool wallBFound = false;
        if (wallA != nullptr)
            wallAFound = Main::foundWall(*wallA);
        if (wallB != nullptr)
            wallBFound = Main::foundWall(*wallB);
        if (boxA && boxB && !Config::collision)
            contact->SetEnabled(false);
        if (boxA && boxB)
            return;
        if (boxA && wallAFound) {
            boxA->collision_locs->push_back(getCollisionPoint(boxA));
        }
        if (boxA && wallBFound) {
            boxA->collision_locs->push_back(getCollisionPoint(boxA));
        }
        if (boxB && wallAFound) {
            boxB->collision_locs->push_back(getCollisionPoint(boxB));
        }
        if (boxB && wallBFound) {
            boxB->collision_locs->push_back(getCollisionPoint(boxB));
        }
    }
};


#endif // PHYSICSSIM_MAIN_H
