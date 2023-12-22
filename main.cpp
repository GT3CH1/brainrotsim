#include "main.h"

void Main::setup() {
    srandom(time(nullptr));
    Keyboard::Keyboard_Init();
    Renderer::init();
    world = new b2World(b2Vec2(0, 9.8));
    auto *contactListener = new ContactListener();
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
    const int world_top = Config::SCREEN_HEIGHT / Renderer::WINDOW_SCALE;
    const int world_right = Config::SCREEN_WIDTH / Renderer::WINDOW_SCALE;

    bottom = new Wall(0, world_top - 1, world_right, 1.0f, world);
    top = new Wall(0, -1, world_right, 1.0f, world);
    left = new Wall(-1, 0, 1, world_top, world);
    right = new Wall(world_right, 0, 1, world_top, world);

    Renderer::addLayer(MAIN_LAYER);
    Renderer::addLayer(OUTLINE_LAYER);

    Renderer::addLayer(LINE_LAYER);
    Renderer::setBlendMode(MAIN_LAYER, SDL_BLENDMODE_ADD);
    Renderer::setBlendMode(OUTLINE_LAYER, SDL_BLENDMODE_ADD);
    Renderer::setBlendMode(LINE_LAYER, SDL_BLENDMODE_NONE);
}

void Main::reset_simulation() {
    for (const auto box: boxes) {
        world->DestroyBody(box->body);
    }
    boxes.clear();

    pending_boxes->clear();
    Renderer::clearAllLayers();
    Renderer::setRenderLayer(MAIN_LAYER);
}

void Main::handle_keyboard() {
    if (Keyboard::keyWasPressed(SDLK_SPACE))
        Config::paused = !Config::paused;
    if (Keyboard::keyWasPressed(SDLK_r)) {
        reset_simulation();
    }
    if (Keyboard::keyWasPressed(SDLK_l)) {
        Config::line_mode = !Config::line_mode;
        Renderer::clearLayer(LINE_LAYER);
    }
    if (Keyboard::keyWasPressed(SDLK_c))
        Config::collision = !Config::collision;
    if (Keyboard::keyWasPressed(SDLK_b))
        boxes.push_back(new Box(world));
    if (Keyboard::keyWasPressed(SDLK_n))
        for (int i = 0; i < 100; i++)
            boxes.push_back(new Box(world));
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
    if (Keyboard::keyWasPressed(SDLK_o)) {
        Config::outline = !Config::outline;
        Renderer::clearLayer(OUTLINE_LAYER);
    }
    if (Keyboard::keyWasPressed(SDLK_s))
        Config::sound = !Config::sound;
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

void Main::draw_outline(const SDL_FRect *rect, const b2Body *body) {
    constexpr auto offset = 4;
    SDL_FRect dummy;
    dummy.x = offset / 2.0f + (body->GetPosition().x - rect->w / 2.0f) * Renderer::WINDOW_SCALE;
    dummy.y = offset / 2.0f + (body->GetPosition().y - rect->h / 2.0f) * Renderer::WINDOW_SCALE;
    dummy.w = -offset + rect->w * Renderer::WINDOW_SCALE;
    dummy.h = dummy.w;
    Renderer::drawRect(&dummy, false);
}

void Main::drawLineToBox(const Box *box) {
    const auto center = box->body->GetWorldCenter();
    Renderer::setDrawColor(box->color);
    const auto p1 = SDL_FPoint{center.x * Renderer::WINDOW_SCALE, center.y * Renderer::WINDOW_SCALE};
    const auto p2 = SDL_FPoint{Config::SCREEN_CENTER_X * Renderer::WINDOW_SCALE,
                               Config::SCREEN_CENTER_Y * Renderer::WINDOW_SCALE};
    Renderer::drawLine(&p1, &p2);
}

void Main::addNewBoxes() {
    for (const auto box: *pending_boxes) {
        auto _box = new Box(world, box->x, box->y, box->w, box->h, box->color);
        boxes.push_back(_box);
        if (boxes.size() >= Config::MAX_BOXES) {
            break;
        }
    }
    pending_boxes->clear();
}

void Main::render_boxes(bool &box_has_audio) {
    SDL_FRect dummy;
    Renderer::setRenderLayer(MAIN_LAYER);
    for (const auto box: boxes) {
        const uint32 r = box->color.red();
        const uint32 g = box->color.green();
        const uint32 b = box->color.blue();
        const auto rect = box->rect;
        const auto body = box->body;
        Renderer::setDrawColor(r, g, b);
        dummy.x = (body->GetPosition().x - rect->w / 2.0f) * Renderer::WINDOW_SCALE;
        dummy.y = (body->GetPosition().y - rect->h / 2.0f) * Renderer::WINDOW_SCALE;
        dummy.w = rect->w * Renderer::WINDOW_SCALE;
        dummy.h = dummy.w;
        Renderer::drawRect(&dummy);
        box->num_audio_frames--;
        if (box->num_audio_frames > 0) {
            box_has_audio = true;
            beeper.setWaveTone(box->tone);
            beeper.setSoundOn(Config::sound);
        }
    }

    if (Config::outline) {
        Renderer::setRenderLayer(OUTLINE_LAYER);
        Renderer::setDrawColor(BLACK);
        Renderer::clearRenderer();
        Renderer::setDrawColor(WHITE);
        for (const Box *box: boxes) {
            const auto rect = box->rect;
            const auto body = box->body;
            draw_outline(rect, body);
        }
        Renderer::setRenderLayer(MAIN_LAYER);
    }
    if (Config::line_mode) {
        Renderer::setRenderLayer(LINE_LAYER);
        Renderer::setDrawColor(BLACK);
        Renderer::clearRenderer();
        for (const auto box: boxes)
            drawLineToBox(box);
        Renderer::setRenderLayer(MAIN_LAYER);
    }
}

void Main::run() {
    Renderer::setRenderLayer(MAIN_LAYER);
    while (Config::runner) {
        handle_key_event();
        handle_keyboard();
        if (Config::paused) {
            beeper.setSoundOn(false);
            continue;
        }

        if (boxes.empty())
            boxes.push_back(new Box(world));

        Renderer::setDrawColor(BLACK);
        if (Config::clear_on_frame) {
            Renderer::clearAllLayers();
        }
        if (boxes.size() < Config::MAX_BOXES)
            addNewBoxes();

        auto box_has_audio = false;
        render_boxes(box_has_audio);
        if (!box_has_audio)
            beeper.setSoundOn(false);
        Renderer::setDrawColor(BLACK);
        Renderer::copyAllLayersToRenderer();
        Renderer::present();
        world->Step(WORLD_STEP, NUM_VEL_ITERATIONS, NUM_POS_ITERATIONS);
        Renderer::setRenderLayer(MAIN_LAYER);
    }
}
