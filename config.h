//
// Created by GT3CH1 on 11/20/23.
//

#ifndef PHYSICSSIM_CONFIG_H
#define PHYSICSSIM_CONFIG_H

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
    static bool outline;
    static int SCREEN_WIDTH;
    static int SCREEN_HEIGHT;
    static int SCREEN_CENTER_X;
    static int SCREEN_CENTER_Y;
    static int MAX_X;
    static int MAX_Y;
    static int MAX_BOXES;
    static bool sound;
};


#endif //PHYSICSSIM_CONFIG_H
