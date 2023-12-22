//
// Created by GT3CH1 on 12/17/23.
// Copyright (c) 2023 GT3CH1. All rights reserved.
#include "simrenderer.h"

SDL_Renderer *Renderer::renderer = nullptr;
SDL_Surface *Renderer::render_surface = nullptr;
SDL_Texture *Renderer::render_target = nullptr;
SDL_Window *Renderer::window = nullptr;
SDL_Rect *Renderer::bg = nullptr;
std::map<std::string, SDL_Texture *> Renderer::layers;
std::string MAIN_LAYER = "_main_renderer";
/**
 * Initializes the window, renderer, and sets the render scale.
 */
void Renderer::init() {
    window = SDL_CreateWindow("Brain Rot Sim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                              SDL_WINDOW_SHOWN);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);

    if (window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }

    if (renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }

    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    printf("%s", info.name);
    SDL_RenderSetScale(renderer, RENDER_SCALE_X, RENDER_SCALE_Y);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    setupScreen();
    addLayer(MAIN_LAYER);
}

void Renderer::addLayer(const std::string &name) {
    auto _texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                      Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT);
    layers.insert(std::pair(name, _texture));
}

void Renderer::setRenderLayer(const std::string &name) {
    const auto _texture = layers.at(name);
    if (_texture == nullptr) {
        std::cerr << "Layer " << name << " does not exist!" << std::endl;
        return;
    }
    SDL_SetRenderTarget(renderer, _texture);
}

void Renderer::setNullRenderLayer() { SDL_SetRenderTarget(renderer, nullptr); }
void Renderer::setBlendMode(const std::string &name, const SDL_BlendMode blendMode) {
    const auto _texture = layers.at(name);
    SDL_SetTextureAlphaMod(_texture, 255);
    SDL_SetTextureColorMod(_texture, 255, 255, 255);
    SDL_SetTextureBlendMode(_texture, blendMode);
}

void Renderer::clearRenderer() { SDL_RenderClear(renderer); }

void Renderer::clearAllLayers() {
    const auto _current = SDL_GetRenderTarget(renderer);
    setDrawColor(BLACK);
    for (auto &layer: layers) {
        setRenderLayer(layer.first);
        clearRenderer();
    }
    SDL_SetRenderTarget(renderer, _current);
}

void Renderer::setDrawColor(const Color &c) { setDrawColor(c.red(), c.blue(), c.green()); }

void Renderer::setDrawColor(const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Renderer::drawRect(const SDL_Rect *rect, const bool fill) {
    if (fill)
        SDL_RenderFillRect(renderer, rect);
    else
        SDL_RenderDrawRect(renderer, rect);
}

void Renderer::drawRect(const SDL_FRect *rect, const bool fill) {
    if (fill)
        SDL_RenderFillRectF(renderer, rect);
    else
        SDL_RenderDrawRectF(renderer, rect);
}

void Renderer::copyLayerToRenderer(const std::string &name) {
    const auto _texture = layers.at(name);
    if (_texture == nullptr) {
        std::cerr << "Layer " << name << " does not exist!" << std::endl;
        return;
    }
    SDL_RenderCopy(renderer, _texture, nullptr, nullptr);
}
void Renderer::copyAllLayersToRenderer() {
    setRenderLayer(MAIN_LAYER);
    for (auto &layer: layers) {
        copyLayerToRenderer(layer.first);
    }
    setNullRenderLayer();
    copyLayerToRenderer(MAIN_LAYER);
}


void Renderer::present() {
    SDL_RenderPresent(renderer);
    setRenderLayer(MAIN_LAYER);
}


void Renderer::setupScreen() {
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    Config::SCREEN_WIDTH = DM.w;
    Config::SCREEN_HEIGHT = DM.h;
    clearRenderer();
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    setDrawColor(BLACK);
    bg = new SDL_Rect{0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT};
    const int max_y = (Config::SCREEN_HEIGHT) / WINDOW_SCALE;
    const int max_x = (Config::SCREEN_WIDTH) / WINDOW_SCALE;
    Config::SCREEN_CENTER_X = max_x / 2;
    Config::SCREEN_CENTER_Y = max_y / 2;
}
void Renderer::clearLayer(const std::string &name) {
    auto const curr_layer = SDL_GetRenderTarget(renderer);
    setRenderLayer(name);
    clearRenderer();
    SDL_SetRenderTarget(renderer, curr_layer);
}
