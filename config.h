//
// Created by GT3CH1 on 11/20/23.
//

#ifndef PHYSICSSIM_CONFIG_H
#define PHYSICSSIM_CONFIG_H
#include "Color.h"

class Config
{
public:
    static bool grow;
    static bool runner;
    static bool paused;
    static bool line_mode;
    static bool collision;
    static bool fullscreen;
    static bool color_change_on_bounce;
    static bool debug;
    static bool clear_on_frame;
    static bool smear_line;
    static bool outline;
    static bool wireframe;
    static float SCREEN_WIDTH;
    static float SCREEN_HEIGHT;
    static float SCREEN_CENTER_X;
    static float SCREEN_CENTER_Y;
    static int MAX_X;
    static int MAX_Y;
    static int MAX_BOXES;
    static bool sound;
    static bool same_color_mode;
    static bool quad_line_mode;
    static bool corner_line_mode;
    static bool render_boxes;
};


#endif //PHYSICSSIM_CONFIG_H
