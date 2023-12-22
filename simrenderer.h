//
// Created by GT3CH1 on 12/15/23.
//

#ifndef PHYSICSSIM_RENDERER_H
#define PHYSICSSIM_RENDERER_H

#include <SDL2/SDL.h>
#include <map>

#include "Color.h"

class Renderer {
private:
    static SDL_Renderer *renderer;
    static SDL_Surface *render_surface;
    static SDL_Texture *render_target;
    static SDL_Window *window;
    static SDL_Rect *bg;
    static std::map<std::string, SDL_Texture *> layers;

public:
    inline static int RENDER_SCALE_X = 100;
    inline static int RENDER_SCALE_Y = 100;
    inline static float WINDOW_SCALE = 10.0f;
    inline static std::string _RENDER_MAIN_LAYER = "_main_renderer";
    static void init();

    /**
     * \brief Creates a new layer with the given name.
     * \param name The name of the render layer.
     */
    static void addLayer(const std::string &name);

    /**
     * \brief Changes what layer the renderer is currently drawing to.
     * \param name The name of the render layer to target.
     */
    static void setRenderLayer(const std::string &name);

    /**
     * \brief Sets the renderer to draw to the default layer.
     */
    static void setNullRenderLayer();

    /**
     * \brief Sets the blend mode of the given layer.
     * \param name The name of the layer to change the blend mode of.
     * \param blendMode The blend mode to set the layer to.
     */
    static void setBlendMode(const std::string &name, SDL_BlendMode blendMode);

    /**
     * \brief Clears the renderer.
     */
    static void clearRenderer();

    /**
     * \brief Clears all layers.
     */
    static void clearAllLayers();

    /**
     * \brief Sets the draw color of the renderer.
     * \param c The color to set the renderer to.
     */
    static void setDrawColor(const Color &c);

    /**
     * \brief Sets the draw color of the renderer.
     * \param r The red value of the color.
     * \param g The green value of the color.
     * \param b The blue value of the color.
     * \param a The alpha value of the color. Defaults to 255.
     */
    static void setDrawColor(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255);

    /**
     * \brief Draws a rectangle to the renderer.
     * \param rect The rectangle to draw.
     * \param fill Whether or not to fill the rectangle. Default is true.
     */
    static void drawRect(const SDL_Rect *rect, bool fill = true);

    /**
     * \brief Draws a rectangle to the renderer.
     * \param rect The rectangle to draw.
     * \param fill Whether or not to fill the rectangle. Default is true.
     */
    static void drawRect(const SDL_FRect *rect, bool fill = true);

    /**
     * \brief Copies the given layer to the renderer.
     * \param name The name of the layer to copy.
     */
    static void copyLayerToRenderer(const std::string &name);

    /**
     * \brief Copies all layers to the renderer.
     */
    static void copyAllLayersToRenderer();

    /**
     * \brief Presents the renderer.
     */
    static void present();

    /**
     * \brief Sets up the screen.
     */
    static void setupScreen();

    /**
     * \brief Clears the given layer.
     * \param name The name of the layer to clear.
     */
    static void clearLayer(const std::string &name);

    /**
     * \brief Draws a line from p1 to p2.
     * \param p1 The first point of the line.
     * \param p2 The second point of the line.
     */
    static void drawLine(const SDL_FPoint *p1, const SDL_FPoint *p2);
};

#endif // PHYSICSSIM_RENDERER_H
