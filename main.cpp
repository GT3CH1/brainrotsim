#include "main.h"

void Main::setup() {
    srandom(time(nullptr));
    Keyboard::Keyboard_Init();
    Renderer::init();
    world = new b2World(b2Vec2(0, 9.8));
    auto* contactListener = new ContactListener();
    world->SetContactListener(contactListener);

    setupWorld();
    beeper.initializeAudio();
    beeper.setWaveTone(440);
    beeper.setSoundOn(false);
    beeper.setWaveType(0);
    boxes = std::vector<Box *>();
}

int main() {
    Main::setup();
    Main::run();
}

void Main::setupWorld() {
    boxes.clear();
    // delete old walls
    if (bottom != nullptr)
        world->DestroyBody(bottom->body);
    if (top != nullptr)
        world->DestroyBody(top->body);
    if (left != nullptr)
        world->DestroyBody(left->body);
    if (right != nullptr)
        world->DestroyBody(right->body);
    const int world_top = Config::SCREEN_HEIGHT / 10;
    const int world_right = Config::SCREEN_WIDTH / 10;

    bottom = new Wall(0, world_top - 1, world_right, 1.0f, world);
    top = new Wall(0, -1, world_right, 1.0f, world);
    left = new Wall(-1, 0, 1, world_top, world);
    right = new Wall(world_right, 0, 1, world_top, world);

    Renderer::addLayer("main");
}

void Main::reset_simulation() {
    for (const auto box: boxes) {
        world->DestroyBody(box->body);
    }
    boxes.clear();

    pending_boxes->clear();
    SDL_RenderClear(Renderer::renderer);
    SDL_SetRenderDrawColor(Renderer::renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(Renderer::renderer, Renderer::bg);
}

void Main::handle_keyboard() {
    if (Keyboard::keyWasPressed(SDLK_SPACE))
        Config::paused = !Config::paused;
    if (Keyboard::keyWasPressed(SDLK_r)) {
        reset_simulation();
    }
    if (Keyboard::keyWasPressed(SDLK_l))
        Config::line_mode = !Config::line_mode;
    if (Keyboard::keyWasPressed(SDLK_c))
        Config::collision = !Config::collision;
    if (Keyboard::keyWasPressed(SDLK_b))
        boxes.push_back(new Box(world, Renderer::renderer));
    if (Keyboard::keyWasPressed(SDLK_n))
        for (int i = 0; i < 100; i++)
            boxes.push_back(new Box(world, Renderer::renderer));
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

void Main::handle_key_event() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT: {
                Config::runner = false;
                break;
            }
            case SDL_KEYDOWN: {
                Keyboard::setKeyDown(static_cast<SDL_KeyCode>(e.key.keysym.sym), true);
                break;
            }
            case SDL_KEYUP: {
                Keyboard::setKeyDown(static_cast<SDL_KeyCode>(e.key.keysym.sym), false);
                break;
            }
            default:
                break;
        }
    }
}

void Main::draw_outline(SDL_FRect& dummy, const SDL_FRect* rect, const b2Body* body) {
    SDL_SetRenderDrawColor(Renderer::renderer, 255, 255, 255, 127);
    constexpr auto offset = 4;
    dummy.x = offset / 2.0f + (body->GetPosition().x - rect->w / 2.0f) * 10;
    dummy.y = offset / 2.0f + (body->GetPosition().y - rect->h / 2.0f) * 10;
    dummy.w = -offset + rect->w * 10;
    dummy.h = dummy.w;
    SDL_RenderDrawRectF(Renderer::renderer, &dummy);
}

void Main::drawLineToBox(const Box* box, const uint32 r, const uint32 g, const uint32 b) {
    const auto center = box->body->GetWorldCenter();
    // SDL_SetRenderDrawColor(Renderer::renderer, r, g, b, 255);
    Renderer::setDrawColor(r, g, b);
    SDL_RenderDrawLineF(Renderer::renderer, center.x * 10, center.y * 10, Config::SCREEN_CENTER_X * 10,
                        Config::SCREEN_CENTER_Y * 10);
}

void Main::addNewBoxes() {
    for (const auto box: *pending_boxes) {
        auto _box = new Box(world, Renderer::renderer, box->x, box->y, box->w, box->h, box->color);
        boxes.push_back(_box);
        if (boxes.size() >= Config::MAX_BOXES) {
            break;
        }
    }
    pending_boxes->clear();
}

void Main::render_boxes(bool& box_has_audio) {
    SDL_FRect dummy;
    for (const auto box: boxes) {
        const uint32 r = box->color.red();
        const uint32 g = box->color.green();
        const uint32 b = box->color.blue();
        const auto rect = box->rect;
        const auto body = box->body;
        // SDL_SetRenderDrawColor(Renderer::renderer, r, g, b, 255);
        Renderer::setDrawColor(r, g, b);
        dummy.x = (body->GetPosition().x - rect->w / 2.0f);
        dummy.y = (body->GetPosition().y - rect->h / 2.0f);
        dummy.w = rect->w * 10;
        dummy.h = dummy.w;
        // SDL_RenderFillRectF(Renderer::renderer, &dummy);
        Renderer::drawRect(dummy);
        // draw outline
        if (Config::outline) {
            draw_outline(dummy, rect, body);
        }

        // SDL_SetRenderTarget(Renderer::renderer, Renderer::render_target);
        Renderer::setRenderLayer("main");
        box->num_audio_frames--;
        if (box->num_audio_frames > 0) {
            box_has_audio = true;
            beeper.setWaveTone(box->tone);
            beeper.setSoundOn(true);
        }

        if (Config::line_mode)
            drawLineToBox(box, r, g, b);
    }
}

void Main::run() {
    while (Config::runner) {
        // SDL_SetRenderTarget(Renderer::renderer, Renderer::render_target);
        Renderer::setRenderLayer("main");
        handle_key_event();
        handle_keyboard();
        if (Config::paused) {
            beeper.setSoundOn(false);
            continue;
        }

        if (boxes.empty())
            boxes.push_back(new Box(world, Renderer::renderer));

        // SDL_SetRenderDrawColor(Renderer::renderer, 0, 0, 0, 255);
        Renderer::setDrawColor(255, 0, 0);
        // if (Config::clear_on_frame) {
        // SDL_RenderClear(Renderer::renderer);
        // SDL_RenderFillRect(Renderer::renderer, Renderer::bg);
        // }
        if (boxes.size() < Config::MAX_BOXES)
            addNewBoxes();

        auto box_has_audio = false;
        render_boxes(box_has_audio);
        if (!box_has_audio)
            beeper.setSoundOn(false);
        Renderer::setDrawColor(255, 255, 0);
        // SDL_SetRenderDrawColor(Renderer::renderer, 0, 255, 255, 255);
        // SDL_SetRenderTarget(Renderer::renderer, nullptr);
        Renderer::setNullRenderLayer();
        // SDL_RenderCopyF(Renderer::renderer, Renderer::render_target, nullptr, nullptr);
        Renderer::copyLayerToRenderer("main");
        if (Keyboard::keyDown(SDLK_TAB)) {
            // show what features are enabled
            SDL_SetRenderDrawColor(Renderer::renderer, 255, 255, 255, 255);
            SDL_RenderDrawLineF(Renderer::renderer, 0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT);
            SDL_RenderDrawLineF(Renderer::renderer, Config::SCREEN_WIDTH, 0, 0, Config::SCREEN_HEIGHT);
        }
        SDL_RenderPresent(Renderer::renderer);
        world->Step(1 / 60.0f, 1, 1);
    }
}
