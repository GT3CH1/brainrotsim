#include <iostream>
#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include <vector>
#include "box.hpp"
#include "Keyboard.hpp"

#include "main.h"
#include "config.h"

class ContactListener : public b2ContactListener {
    void BeginContact(b2Contact *contact) override {
        b2Fixture *fixtureA = contact->GetFixtureA();
        b2Fixture *fixtureB = contact->GetFixtureB();
        Box *boxA = reinterpret_cast<Box *>(fixtureA->GetUserData().pointer);
        Box *boxB = reinterpret_cast<Box *>(fixtureB->GetUserData().pointer);
        if (boxA && boxB)
            return;
        if (boxA) {
            Box::onCollision(Main::pending_boxes);
            if (Config::grow) {
                boxA->rect->w *= 1.01f;
                boxA->rect->h *= 1.01f;
            }
            auto shape = (b2PolygonShape *) boxA->body->GetFixtureList()->GetShape();
            shape->SetAsBox(boxA->rect->w / 2.0f, boxA->rect->h / 2.0f);
            if (boxA->num_audio_frames <= 0) {
                boxA->num_audio_frames = 10;
                auto tone = 1760 / boxA->rect->w;
                if (tone < 27.50)
                    tone = 27.50;
                Main::beeper.setWaveTone(tone);
            }
        }
        if (boxB) {
            Box::onCollision(Main::pending_boxes);
            if (Config::grow) {
                boxB->rect->w *= 1.01f;
                boxB->rect->h *= 1.01f;
            }
            if (Config::color_change_on_bounce) {
                boxB->color = Color::randColor();
            }
            auto shape = (b2PolygonShape *) boxB->body->GetFixtureList()->GetShape();
            shape->SetAsBox(boxB->rect->w / 2.0f, boxB->rect->h / 2.0f);
            if (boxB->num_audio_frames <= 0) {
                boxB->num_audio_frames = 10;
                auto tone = 1760 / boxB->rect->w;
                if (tone < 27.50)
                    tone = 27.50;
                Main::beeper.setWaveTone(tone);
                Main::beeper.setSoundOn(true);
            }
        }
    }

    void PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
    override {
        b2Fixture *fixtureA = contact->GetFixtureA();
        b2Fixture *fixtureB = contact->GetFixtureB();
        Box *boxA = reinterpret_cast<Box *>(fixtureA->GetUserData().pointer);
        Box *boxB = reinterpret_cast<Box *>(fixtureB->GetUserData().pointer);
        if (boxA && boxB && !Config::collision)
            contact->SetEnabled(false);
    }
};


int main() {

    Main::setup();
    Main::run();
}

b2World *Main::world;
SDL_Renderer *Main::renderer;
SDL_Window *Main::window;
Beeper Main::beeper;
Wall *Main::bottom;
Wall *Main::top;
Wall *Main::left;
Wall *Main::right;
std::vector<Box *> Main::boxes;
std::vector<BoxInfo *> *Main::pending_boxes = new std::vector<BoxInfo *>();
SDL_Rect Main::bg;

void Main::setup() {

    srandom(time(nullptr));
    Keyboard::Keyboard_Init();
    window = SDL_CreateWindow("Brain Rot Sim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }
    if (renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }
    SDL_RenderSetScale(renderer, 10, 10);
    boxes = std::vector<Box *>();
    world = new b2World(b2Vec2(0, 9.8));
    SDL_Init(SDL_INIT_EVERYTHING);
    boxes.push_back(new Box(world, renderer, 32, 24, 1, 1));
    auto *contactListener = new ContactListener();
    world->SetContactListener(contactListener);
    // set world coordinates to screen coordinates

    // create a 4.8 x 6.4 meter box at the bottom of the screen
    float time_step = 1.0f / 60.0f;

    SDL_Event e;
    bg.x = 0;
    bg.y = 0;
    bg.w = 640.0f;
    bg.h = 480.0f;
    Config::runner = true;
    Config::paused = true;


    setScreenDimensions(false);

    Main::beeper.initializeAudio();
    Main::beeper.setWaveTone(440);
    Main::beeper.setSoundOn(false);
    Main::beeper.setWaveType(0);
}

void Main::setScreenDimensions(bool fullscreen) {
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    Config::SCREEN_WIDTH = DM.w;
    Config::SCREEN_HEIGHT = DM.h;
    float max_x = 64;
    float max_y = 48;
    boxes.clear();
    boxes.push_back(new Box(world, renderer));
    if (fullscreen) {
        max_y = Config::SCREEN_HEIGHT / 10.0f;
        max_x = Config::SCREEN_WIDTH / 10.0f;

    }
    Config::SCREEN_CENTER_X = max_x / 2;
    Config::SCREEN_CENTER_Y = max_y / 2;
    printf("MaxX: %f, MaxY: %f\n", max_x, max_y);

    // delete old walls
    if (bottom != nullptr)
        world->DestroyBody(bottom->body);
    if (top != nullptr)
        world->DestroyBody(top->body);
    if (left != nullptr)
        world->DestroyBody(left->body);
    if (right != nullptr)
        world->DestroyBody(right->body);

    delete bottom;
    delete top;
    delete left;
    delete right;

    bottom = new Wall(0, max_y - 1.0f, max_x, 1.0f, world);
    top = new Wall(0, -1, max_x, 1.0f, world);
    left = new Wall(-1, 0, 1, max_y, world);
    right = new Wall(max_x, 0, 1, max_y, world);
}

void Main::run() {

    while (Config::runner) {

        if (boxes.size() < Config::MAX_BOXES) {
            for (auto box: *pending_boxes) {
                auto new_boc = new Box(world, renderer, box->x, box->y, box->w, box->h, box->color);
                boxes.push_back(new_boc);
                if (boxes.size() >= Config::MAX_BOXES) {
                    break;
                }
            }
            pending_boxes->clear();
        }


        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: {
                    Config::runner = false;
                    break;
                }
                case SDL_KEYDOWN: {
                    Keyboard::setKeyDown((SDL_KeyCode) e.key.keysym.sym, true);
                    break;
                }
                case SDL_KEYUP: {
                    Keyboard::setKeyDown((SDL_KeyCode) e.key.keysym.sym, false);
                    break;
                }
            }
        }
        if (Keyboard::keyWasPressed(SDLK_SPACE))
            Config::paused = !Config::paused;
        if (Keyboard::keyWasPressed(SDLK_r)) {
            for (auto box: boxes) {
                world->DestroyBody(box->body);
            }
            boxes.clear();
            boxes.push_back(new Box(world, renderer, arc4random() % 32, arc4random() % 24, 1, 1));
            pending_boxes->clear();
        }
        if (Keyboard::keyWasPressed(SDLK_l))
            Config::line_mode = !Config::line_mode;
        if (Keyboard::keyWasPressed(SDLK_c))
            Config::collision = !Config::collision;
        if (Keyboard::keyWasPressed(SDLK_b))
            boxes.push_back(new Box(world, renderer));
        if (Keyboard::keyWasPressed(SDLK_n)) {
            for (int i = 0; i < 100; i++) {
                boxes.push_back(new Box(world, renderer));
            }
        }
        if (Keyboard::keyWasPressed(SDLK_ESCAPE))
            Config::runner = false;
        if (Keyboard::keyWasPressed(SDLK_g))
            Config::grow = !Config::grow;
        if (Keyboard::keyWasPressed(SDLK_f)) {
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            setScreenDimensions(true);
        }
        if (Keyboard::keyWasPressed(SDLK_m)) {
            SDL_SetWindowFullscreen(window, 0);
            setScreenDimensions(false);
        }
        if (Keyboard::keyWasPressed(SDLK_x)) {
            Config::color_change_on_bounce = !Config::color_change_on_bounce;
        }
        Keyboard::update();
        if (Config::paused) {
            continue;
        }
//            SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &bg);
        auto box_has_audio = false;
        for (auto it = boxes.begin(); it != boxes.end(); ++it) {
            Box *box = *it;
            // draw line from center of box to middle of screen
            if (Config::line_mode) {
                auto center = box->body->GetWorldCenter();
                SDL_SetRenderDrawColor(renderer, box->color.red(), box->color.green(), box->color.blue(), 255);
                SDL_RenderDrawLineF(renderer, center.x, center.y, Config::SCREEN_CENTER_X, Config::SCREEN_CENTER_Y);
            }
            box->update(renderer);
            if (box->num_audio_frames > 0)
                box_has_audio = true;
        }
        if (!box_has_audio) {
            Main::beeper.setSoundOn(false);
        }
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        SDL_RenderFillRectF(renderer, &bottom->rect);
        SDL_RenderFillRectF(renderer, &left->rect);
        SDL_RenderFillRectF(renderer, &right->rect);
        SDL_RenderFillRectF(renderer, &top->rect);

        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderPresent(renderer);
        world->Step(1 / 60.0f, 1, 1);
    }

}