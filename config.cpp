//
// Created by GT3CH1 on 12/17/23.
// Copyright (c) 2023 GT3CH1. All rights reserved.
#include "config.h"
bool Config::paused = false;
bool Config::line_mode = false;
bool Config::collision = false;
bool Config::fullscreen = true;
bool Config::color_change_on_bounce = false;
bool Config::clear_on_frame = true;
bool Config::debug = true;
bool Config::outline = false;
bool Config::runner = true;
bool Config::grow = true;
int Config::SCREEN_WIDTH = 64;
int Config::SCREEN_HEIGHT = 48;
int Config::SCREEN_CENTER_X = Config::SCREEN_WIDTH / 2;
int Config::SCREEN_CENTER_Y = Config::SCREEN_HEIGHT / 2;
int Config::MAX_BOXES = 60;