#include "main.h"

void Main::setup() {
    srand(time(nullptr));
    Keyboard::Keyboard_Init();

    Renderer::init();
    world = new b2World(b2Vec2(0, GRAVITY));
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
    return 0;
}

int WinMain() { return main(); }

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
    const float world_top = Config::SCREEN_HEIGHT / Renderer::WINDOW_SCALE;
    const float world_right = Config::SCREEN_WIDTH / Renderer::WINDOW_SCALE;

    bottom = new Wall(0, world_top, world_right, 1.0f, world);
    top = new Wall(0, -1, world_right, 1.0f, world);
    left = new Wall(-1, 0, 1, world_top, world);
    right = new Wall(world_right, 0, 1, world_top, world);

    LINE_LAYER = Renderer::addLayer();
    BOX_LAYER = Renderer::addLayer();
    OUTLINE_LAYER = Renderer::addLayer();

    Renderer::setBlendMode(LINE_LAYER, SDL_BLENDMODE_NONE);
    Renderer::setBlendMode(BOX_LAYER, normal_blend_mode);
    Renderer::setBlendMode(OUTLINE_LAYER, normal_blend_mode);
}

void Main::reset_simulation() {
    for (const auto box: boxes) {
        world->DestroyBody(box->body);
    }
    boxes.clear();

    pending_boxes->clear();
    Renderer::clearAllLayers();
    Renderer::setRenderLayer(BOX_LAYER);
}

void Main::spawnBox() {
    auto const box = new Box(world);
    boxes.push_back(box);
    if (Config::same_color_mode)
        box->color = boxes[0]->color;
}
void Main::handleWireFrameToggle() {
    Config::wireframe = !Config::wireframe;
    Config::outline = Config::wireframe;
    Config::line_mode = Config::wireframe;
    Renderer::clearLayer(OUTLINE_LAYER);
    Renderer::clearLayer(LINE_LAYER);
    if (Config::wireframe) {
        Config::smear_line = true;
        Config::outline = true;
        Config::line_mode = true;
        if (current_linemode == LineMode::NONE)
            current_linemode = LineMode::CENTER;
    } else {
        Config::smear_line = false;
    }
    SDL_BlendMode _customBlend;
    if (Config::wireframe) {
        _customBlend = wireframe_blend_mode;
    } else {
        _customBlend = normal_blend_mode;
    }
    Renderer::setBlendMode(LINE_LAYER, SDL_BLENDMODE_NONE);
    Renderer::setBlendMode(BOX_LAYER, _customBlend);
    Renderer::setBlendMode(OUTLINE_LAYER, _customBlend);
}
void Main::handleKeybind() {
    if (Keyboard::keyWasPressed(SDLK_SPACE))
        Config::paused = !Config::paused;
    if (Keyboard::keyWasPressed(SDLK_r)) {
        reset_simulation();
    }
    if (Keyboard::keyWasPressed(SDLK_l) && !Keyboard::shiftKeyDown()) {
        handleLineModeToggle();
        Renderer::clearLayer(LINE_LAYER);
    }
    if (Keyboard::keyWasPressed(SDLK_l) && Keyboard::shiftKeyDown()) {
        Config::smear_line = !Config::smear_line;
        Renderer::clearLayer(LINE_LAYER);
    }
    if (Keyboard::keyWasPressed(SDLK_c))
        Config::collision = !Config::collision;
    if (Keyboard::keyWasPressed(SDLK_b)) {
        spawnBox();
    }
    if (Keyboard::keyWasPressed(SDLK_n))
        for (int i = 0; i < 100; i++) {
            spawnBox();
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
    if (Keyboard::keyWasPressed(SDLK_o)) {
        Config::outline = !Config::outline;
        Renderer::clearLayer(OUTLINE_LAYER);
    }
    if (Keyboard::keyWasPressed(SDLK_s))
        Config::sound = !Config::sound;
    if (Keyboard::keyWasPressed(SDLK_w)) {
        handleWireFrameToggle();
    }
    if (Keyboard::keyWasPressed(SDLK_e))
        Config::same_color_mode = !Config::same_color_mode;
    if (Keyboard::keyWasPressed(SDLK_h)) {
        Config::render_boxes = !Config::render_boxes;
    }
    Keyboard::update();
}

void Main::onKeyPress() {
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

void Main::draw_outline(const Box *box) {
    auto const _b = box->getScreenRect();
    Renderer::drawRect(&_b, false);
}

void Main::drawLineToBox(const Box *box) {
    const auto center = box->body->GetWorldCenter();
    const auto p1 = SDL_FPoint{center.x * Renderer::WINDOW_SCALE, center.y * Renderer::WINDOW_SCALE};
    const auto p2 = SDL_FPoint{Config::SCREEN_CENTER_X * Renderer::WINDOW_SCALE,
                               Config::SCREEN_CENTER_Y * Renderer::WINDOW_SCALE};
    Renderer::drawLine(&p1, &p2);
}

void Main::addNewBoxes() {
    for (const auto box: *pending_boxes) {
        auto color = Color::fromInt(box->color);
        if (Config::same_color_mode)
            color = boxes[0]->color;
        auto _box = new Box(world, box->x, box->y, box->w, box->h, color);
        boxes.push_back(_box);
        if (boxes.size() >= Config::MAX_BOXES) {
            break;
        }
    }
    pending_boxes->clear();
}
void Main::handleLineModes() {
    Renderer::setRenderLayer(LINE_LAYER);
    if (!Config::smear_line) {
        Renderer::setDrawColor(BLACK);
        Renderer::clearRenderer();
    }
    for (const auto box: boxes) {
        Renderer::setDrawColor(box->color);
        const auto box_width = box->rect->w / 2 * Renderer::WINDOW_SCALE;
        const auto box_x = box->body->GetPosition().x * Renderer::WINDOW_SCALE;
        const auto box_y = box->body->GetPosition().y * Renderer::WINDOW_SCALE;
        const auto left_x = box_x - box_width;
        const auto right_x = box_x + box_width;
        const auto top_y = box_y - box_width;
        const auto bottom_y = box_y + box_width;
        const auto screen_center = SDL_FPoint{Config::SCREEN_CENTER_X * Renderer::WINDOW_SCALE,
                                              Config::SCREEN_CENTER_Y * Renderer::WINDOW_SCALE};
        const auto tl = SDL_FPoint{left_x, top_y};
        const auto bl = SDL_FPoint{left_x, bottom_y};
        const auto tr = SDL_FPoint{right_x, top_y};
        const auto br = SDL_FPoint{right_x, bottom_y};
        if (current_linemode == LineMode::QUAD) {
            Renderer::drawLine(&screen_center, &tl);
            Renderer::drawLine(&screen_center, &tr);
            Renderer::drawLine(&screen_center, &bl);
            Renderer::drawLine(&screen_center, &br);
        }
        if (current_linemode == LineMode::CORNER) {
            constexpr auto tl_corner = SDL_FPoint{0, 0};
            const auto bl_corner = SDL_FPoint{0, Config::SCREEN_CENTER_Y * 20.0f};
            const auto tr_corner = SDL_FPoint{Config::SCREEN_CENTER_X * 20.0f, 0};
            const auto br_corner = SDL_FPoint{Config::SCREEN_CENTER_X * 20.0f, Config::SCREEN_CENTER_Y * 20.0f};
            Renderer::drawLine(&tl_corner, &tl);
            Renderer::drawLine(&tr_corner, &tr);
            Renderer::drawLine(&bl_corner, &bl);
            Renderer::drawLine(&br_corner, &br);
        } else if (current_linemode == LineMode::CENTER) {
            drawLineToBox(box);
        }
    }
    Renderer::setRenderLayer(BOX_LAYER);
}

void Main::render_boxes(bool &box_has_audio) {
    if (Config::outline) {
        Renderer::setRenderLayer(OUTLINE_LAYER);
        Renderer::setDrawColor(BLACK);
        Renderer::clearRenderer();
        Renderer::setDrawColor(WHITE);
        for (const Box *box: boxes) {
            draw_outline(box);
        }
    }
    if (current_linemode != LineMode::NONE) {
        handleLineModes();
    }
    Renderer::setRenderLayer(BOX_LAYER);
    for (const auto box: boxes) {
        if (Config::render_boxes) {
            Renderer::setDrawColor(box->color);
            auto _rect = box->getScreenRect();
            Renderer::drawRect(&_rect);
        }
        box->num_audio_frames--;
        if (box->num_audio_frames > 0) {
            box_has_audio = true;
            beeper.setWaveTone(box->tone);
            beeper.setSoundOn(Config::sound);
        }
    }
}
void Main::handleLineModeToggle() {
    const auto mode = static_cast<LineMode::LineMode>((current_linemode + 1) % NUM_LINE_MODES);
    current_linemode = mode;
}

void Main::run() {
    Renderer::setRenderLayer(BOX_LAYER);
    while (Config::runner) {
        onKeyPress();
        handleKeybind();
        if (Config::paused) {
            beeper.setSoundOn(false);
            continue;
        }

        if (boxes.empty()) {
            auto box = new Box(world);
            boxes.push_back(box);
        }

        Renderer::setDrawColor(BLACK);
        if (Config::clear_on_frame) {
            Renderer::clearLayer(BOX_LAYER);
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
        Renderer::setRenderLayer(BOX_LAYER);
    }
}
