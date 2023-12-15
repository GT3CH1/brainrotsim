#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <box2d/box2d.h>
#include <vector>
#include "box.hpp"
#include "Keyboard.hpp"

#include "main.h"
#include "config.h"

void Main::setup()
{
    srandom(time(nullptr));
    Keyboard::Keyboard_Init();
    window = SDL_CreateWindow("Brain Rot Sim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                              SDL_WINDOW_SHOWN);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);

    if (window == nullptr)
    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }

    if (renderer == nullptr)
    {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }

    if (TTF_Init() < 0)
    {
        std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
    }
    font = TTF_OpenFont("font.ttf", 24);
    if (!font)
    {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
    }
    SDL_RenderSetScale(renderer, 100, 100);
    world = new b2World(b2Vec2(0, 9.8));
    SDL_Init(SDL_INIT_EVERYTHING);
    auto* contactListener = new ContactListener();
    world->SetContactListener(contactListener);
    bg.x = 0;
    bg.y = 0;
    bg.w = 640.0f;
    bg.h = 480.0f;
    Config::runner = true;
    Config::paused = true;

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    setScreenDimensions(true);

    beeper.initializeAudio();
    beeper.setWaveTone(440);
    beeper.setSoundOn(false);
    beeper.setWaveType(0);

    boxes = std::vector<Box*>();
}

int main()
{
    Main::setup();
    Main::run();
}

void Main::setScreenDimensions(const bool fullscreen)
{
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    Config::SCREEN_WIDTH = DM.w;
    Config::SCREEN_HEIGHT = DM.h;
    float max_x = 64;
    float max_y = 48;
    boxes.clear();
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    bg = SDL_Rect{0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &bg);

    if (fullscreen)
    {
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
    bottom = new Wall(0, max_y - 1, max_x, 1.0f, world);
    top = new Wall(0, -1, max_x, 1.0f, world);
    left = new Wall(-1, 0, 1, max_y, world);
    right = new Wall(max_x, 0, 1, max_y, world);


    render_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_TARGET,
                                      max_x * 10, max_y * 10);

    box_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                   SDL_TEXTUREACCESS_TARGET,
                                   max_x * 10, max_y * 10);
}

void Main::handle_keyboard()
{
    if (Keyboard::keyWasPressed(SDLK_SPACE))
    {
        Config::paused = !Config::paused;
    }
    if (Keyboard::keyWasPressed(SDLK_r))
    {
        for (const auto box : boxes)
        {
            world->DestroyBody(box->body);
        }
        boxes.clear();

        pending_boxes->clear();
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &bg);
    }
    if (Keyboard::keyWasPressed(SDLK_l))
        Config::line_mode = !Config::line_mode;
    if (Keyboard::keyWasPressed(SDLK_c))
        Config::collision = !Config::collision;
    if (Keyboard::keyWasPressed(SDLK_b))
        boxes.push_back(new Box(world, renderer));
    if (Keyboard::keyWasPressed(SDLK_n))
    {
        for (int i = 0; i < 100; i++)
        {
            boxes.push_back(new Box(world, renderer));
        }
    }
    if (Keyboard::keyWasPressed(SDLK_ESCAPE))
        Config::runner = false;
    if (Keyboard::keyWasPressed(SDLK_g))
        Config::grow = !Config::grow;
    if (Keyboard::keyWasPressed(SDLK_d))
        Config::debug = !Config::debug;
    if (Keyboard::keyWasPressed(SDLK_x))
        Config::color_change_on_bounce = !Config::color_change_on_bounce;
    if (Keyboard::keyWasPressed(SDLK_f))
        Config::clear_on_frame = !Config::clear_on_frame;
    if (Keyboard::keyWasPressed(SDLK_o))
        Config::outline = !Config::outline;
    Keyboard::update();
}

void Main::handle_key_event()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            {
                Config::runner = false;
                break;
            }
        case SDL_KEYDOWN:
            {
                Keyboard::setKeyDown(static_cast<SDL_KeyCode>(e.key.keysym.sym), true);
                break;
            }
        case SDL_KEYUP:
            {
                Keyboard::setKeyDown(static_cast<SDL_KeyCode>(e.key.keysym.sym), false);
                break;
            }
        default:
            break;
        }
    }
}

void Main::run()
{
    while (Config::runner)
    {
        SDL_SetRenderTarget(renderer, render_target);
        handle_key_event();
        handle_keyboard();
        if (Config::paused)
        {
            beeper.setSoundOn(false);
            continue;
        }
        if (boxes.empty())
        {
            boxes.push_back(new Box(world, renderer));
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        if (Config::clear_on_frame)
        {
            SDL_RenderClear(renderer);
            SDL_RenderFillRect(renderer, &bg);
        }
        if (boxes.size() < Config::MAX_BOXES)
        {
            for (const auto box : *pending_boxes)
            {
                auto _box = new Box(world, renderer, box->x, box->y, box->w, box->h, box->color);
                boxes.push_back(_box);
                if (boxes.size() >= Config::MAX_BOXES)
                {
                    break;
                }
            }
            pending_boxes->clear();
        }


        auto box_has_audio = false;
        // SDL_SetTextureBlendMode(render_target, SDL_BLENDMODE_BLEND);
        // SDL_SetRenderTarget(renderer, box_target);
        // SDL_RenderClear(renderer);
        SDL_FRect dummy;
        for (const auto box : boxes)
        {
            const uint32 r = box->color.red();
            const uint32 g = box->color.green();
            const uint32 b = box->color.blue();
            const auto rect = box->rect;
            const auto body = box->body;
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            dummy.x = (body->GetPosition().x - rect->w / 2.0f) * 10;
            dummy.y = (body->GetPosition().y - rect->h / 2.0f) * 10;
            dummy.w = rect->w * 10;
            dummy.h = dummy.w;
            SDL_RenderFillRectF(renderer, &dummy);
            // draw outline
            if (Config::outline)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 127);
                constexpr auto offset = 4;
                dummy.x = offset / 2.0f + (body->GetPosition().x - rect->w / 2.0f) * 10;
                dummy.y = offset / 2.0f + (body->GetPosition().y - rect->h / 2.0f) * 10;
                dummy.w = -offset + rect->w * 10;
                dummy.h = dummy.w;
                SDL_RenderDrawRectF(renderer, &dummy);
            }

            SDL_SetRenderTarget(renderer, render_target);

            box->num_audio_frames--;
            if (box->num_audio_frames > 0)
            {
                box_has_audio = true;
                beeper.setWaveTone(box->tone);
                beeper.setSoundOn(true);
            }

            if (Config::line_mode)
            {
                const auto center = box->body->GetWorldCenter();
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_RenderDrawLineF(renderer, center.x * 10, center.y * 10, Config::SCREEN_CENTER_X * 10,
                                    Config::SCREEN_CENTER_Y * 10);
            }
        }
        if (!box_has_audio)
        {
            beeper.setSoundOn(false);
        }


        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderCopyF(renderer, render_target, nullptr, nullptr);
        if (Keyboard::keyDown(SDLK_TAB))
        {
            // show what features are enabled
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawLineF(renderer, 0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT);
            SDL_RenderDrawLineF(renderer, Config::SCREEN_WIDTH, 0, 0, Config::SCREEN_HEIGHT);
        }
        SDL_RenderPresent(renderer);

        world->Step(1 / 60.0f, 1, 1);
    }
}
