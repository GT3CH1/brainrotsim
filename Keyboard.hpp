//
// Created by GT3CH1 on 11/20/23.
//

#ifndef PHYSICSSIM_KEYBOARD_H
#define PHYSICSSIM_KEYBOARD_H
//
// Created by GT3CH1 on 10/15/23.
// Copyright (c) 2023 GT3CH1. All rights reserved.

#include <SDL2/SDL_keycode.h>

/**
 * \brief A class that handles key evens from SDL.
 */
class Keyboard {
private:
    // NOTE: Update this if you add more keys
    /**
     * \brief The number of keys.
     */
    static constexpr int NUM_KEYS = 282;
    /**
     * \brief The list of key states for a single frame.
     */
    static bool keys[NUM_KEYS];
    /**
     * \brief The list of key states for the previous frame.
     */
    static bool pressedKeys[NUM_KEYS];

public:
    /**
     * \brief Initializes all keys and key presses to false.
     */
    static void Keyboard_Init() {
        for (int i = 0; i < NUM_KEYS; i++) {
            keys[i] = false;
            pressedKeys[i] = false;
        }
    }

    /**
     * \brief Set the given key to the given value.
     * \param key The key to set.
     * \param isDown The value to set if the key is down.
     */
    static void setKeyDown(const SDL_KeyCode key, const bool isDown) { keys[key & 0xFFF] = isDown; }

    /**
     * \brief Gets whether the key was pressed for at least one frame.
     * \param key The key to check.
     * \return True if the key is currently down.
     */
    static bool keyDown(const SDL_KeyCode key) { return pressedKeys[key & 0xFFF]; }

    /**
     * \brief Gets whether the key was pressed for more than one frame.
     * \param key The key to check.
     * \return True if the key was pressed.
     */
    static bool keyPressed(const SDL_KeyCode key) { return keys[key & 0xFFF]; }

    /**
     * \brief
     * \param key The key to check.
     * \return True if the key was pressed but is no longer down.
     */
    static bool keyWasPressed(const SDL_KeyCode key) {
        const auto pressed = pressedKeys[key & 0xFFF];
        const auto down = keys[key & 0xFFF];
        return pressed && !down;
    }

    /**
     * \brief Updates the state of the keys.
     */
    static void update() {
        for (int i = 0; i < NUM_KEYS; i++) {
            pressedKeys[i] = keys[i];
        }
    }

    /**
     * \brief Gets whether the shift key was pressed for one frame.
     * \return True if either shift key was pressed for one frame.
     */
    static bool shiftKeyDown() { return keyDown(SDLK_LSHIFT) || keyDown(SDLK_RSHIFT); }

    /**
     * \brief Gets whether the shift key was pressed for more than one frame.
     * \return True if either shift key was pressed for more than one frame.
     */
    static bool shiftKeyWasPressed() { return keyWasPressed(SDLK_LSHIFT) || keyWasPressed(SDLK_RSHIFT); }
};

bool Keyboard::keys[NUM_KEYS];
bool Keyboard::pressedKeys[NUM_KEYS];
#endif // PHYSICSSIM_KEYBOARD_H
