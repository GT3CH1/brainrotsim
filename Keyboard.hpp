//
// Created by GT3CH1 on 11/20/23.
//

#ifndef PHYSICSSIM_KEYBOARD_H
#define PHYSICSSIM_KEYBOARD_H
//
// Created by GT3CH1 on 10/15/23.
// Copyright (c) 2023 GT3CH1. All rights reserved.

#include <SDL2/SDL_keycode.h>

class Keyboard {
public:
    static void Keyboard_Init() {
        for (int i = 0; i < 282; i++) {
            keys[i] = false;
            pressedKeys[i] = false;
        }
    }

    static void setKeyDown(SDL_KeyCode key, bool isDown) {
        keys[(int) key & 0xFFF] = isDown;
    }

    static bool keyDown(SDL_KeyCode key) {
        return pressedKeys[(int) key & 0xFFF];

    }

    static bool keyPressed(SDL_KeyCode key) {
        return keys[(int) key & 0xFFF];

    }

    static bool keyWasPressed(SDL_KeyCode key) {
        auto pressed = pressedKeys[(int) key & 0xFFF];
        auto down = keys[(int) key & 0xFFF];
        return pressed && !down;
    }

    static void update() {
        for (int i = 0; i < NUM_KEYS; i++) {
            pressedKeys[i] = keys[i];
        }
    }

    static bool shiftKeyDown() {
        return keyDown(SDLK_LSHIFT) || keyDown(SDLK_RSHIFT);

    }

    static bool shiftKeyWasPressed() {
        return keyWasPressed(SDLK_LSHIFT) || keyWasPressed(SDLK_RSHIFT);
    }

private:
    // NOTE: Update this if you add more keys
    static const int NUM_KEYS = 244;
    static bool keys[NUM_KEYS];
    static bool pressedKeys[NUM_KEYS];

};

bool Keyboard::keys[NUM_KEYS];
bool Keyboard::pressedKeys[NUM_KEYS];
#endif //PHYSICSSIM_KEYBOARD_H
