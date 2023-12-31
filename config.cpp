//
// Created by GT3CH1 on 12/17/23.
// Copyright (c) 2023 GT3CH1. All rights reserved.
#include "config.h"


#include "Color.h"
bool Config::paused = false;
bool Config::line_mode = false;
bool Config::collision = false;
bool Config::fullscreen = true;
bool Config::color_change_on_bounce = false;
bool Config::clear_on_frame = true;
bool Config::debug = true;
bool Config::smear_line = false;
bool Config::outline = false;
bool Config::runner = true;
bool Config::grow = true;
bool Config::sound = false;
bool Config::wireframe = false;
bool Config::same_color_mode = false;
bool Config::quad_line_mode = false;
bool Config::corner_line_mode = false;
bool Config::render_boxes = true;
float Config::SCREEN_WIDTH = 64;
float Config::SCREEN_HEIGHT = 48;
float Config::SCREEN_CENTER_X = SCREEN_WIDTH / 2;
float Config::SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;
int Config::MAX_BOXES = 60;


void Config::handleLineModeToggle() {
    const auto mode = static_cast<LineMode::LineMode>((current_linemode + 1) % LineMode::NUM_LINE_MODES);
    current_linemode = mode;
}
