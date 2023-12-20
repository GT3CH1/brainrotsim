//
// Created by GT3CH1 on 12/15/23.
//

#ifndef PHYSICSSIM_RENDERER_H
#define PHYSICSSIM_RENDERER_H

#include <SDL2/SDL.h>
#include <iostream>
#include <map>

#include "Color.h"
#include "config.h"

class Renderer {
public:
    static SDL_Renderer* renderer;
    static SDL_Surface* render_surface;
    static SDL_Texture* render_target;
    static SDL_Window* window;
    static SDL_Rect* bg;
    static std::map<std::string, SDL_Texture *> layers;
    inline static int RENDER_SCALE_X = 100;
    inline static int RENDER_SCALE_Y = 100;

    static void init();

    /**
     * \brief Creates a new layer with the given name.
     * \param name The name of the render layer.
     * \param blendMode the blend mode of the render layer. Default is to use SDL_BLENDMODE_ADD.
     */
    static void addLayer(const std::string& name, SDL_BlendMode blendMode = SDL_BLENDMODE_ADD);

    /**
     * \brief Changes what layer the renderer is currently drawing to.
     * \param name The name of the render layer to target.
     */
    static void setRenderLayer(const std::string& name);

    /**
     * \brief Sets the renderer to draw to the default layer.
     */
    static void setNullRenderLayer();

    /**
     * \brief Clears the renderer.
     */
    static void clearRenderer();

    static void setDrawColor(Color c);

    static void setDrawColor(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 127);

    static void drawRect(const SDL_Rect& rect, const bool fill = true);

    static void drawRect(const SDL_FRect& rect, const bool fill = true);

    static void copyLayerToRenderer(const std::string& name);


    static void setupScreen();
};

#endif //PHYSICSSIM_RENDERER_H
