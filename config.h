//
// Created by GT3CH1 on 11/20/23.
//

#ifndef PHYSICSSIM_CONFIG_H
#define PHYSICSSIM_CONFIG_H

class Config {
public:
    static bool grow;
    static bool runner;
    static bool paused;
    static bool line_mode;
    static bool collision;
    static bool fullscreen;
    static bool color_change_on_bounce;
    static int SCREEN_WIDTH;
    static int SCREEN_HEIGHT;
    static int SCREEN_CENTER_X;
    static int SCREEN_CENTER_Y;
    static int MAX_X;
    static int MAX_Y;
    static int MAX_BOXES;
};

bool Config::grow = true;
bool Config::runner = true;
bool Config::paused = false;
bool Config::line_mode = false;
bool Config::collision = false;
bool Config::fullscreen = false;
bool Config::color_change_on_bounce = false;
int Config::SCREEN_WIDTH = 64;
int Config::SCREEN_HEIGHT = 48;
int Config::SCREEN_CENTER_X = Config::SCREEN_WIDTH / 2;
int Config::SCREEN_CENTER_Y = Config::SCREEN_HEIGHT / 2;
int Config::MAX_BOXES = 20;
#endif //PHYSICSSIM_CONFIG_H
