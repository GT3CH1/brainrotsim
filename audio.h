//
// Created by GT3CH1 on 11/25/23.
//

#ifndef PHYSICSSIM_AUDIO_H
#define PHYSICSSIM_AUDIO_H

#include <math.h>
#include <SDL2/SDL.h>

typedef struct {
    float current_step;
    float step_size;
    float volume;
} oscillator;

const int SAMPLE_RATE = 44100;
const int BUFFER_SIZE = 4096;

const float A4_OSC = (float) SAMPLE_RATE / 440.00f;

oscillator oscillate(float rate, float volume) {
    oscillator o = {
            .current_step = 0,
            .volume = volume,
            .step_size = static_cast<float>((2 * M_PI) / rate),
    };
    return o;
}

float next(oscillator *os) {
    float ret = sinf(os->current_step);
    os->current_step += os->step_size;
    return ret * os->volume;
}

oscillator *A4_oscillator;

void oscillator_callback(void *userdata, Uint8 *stream, int len) {
    float *fstream = (float *) stream;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        float v = next(A4_oscillator);
        fstream[i] = v;
    }
}

#endif //PHYSICSSIM_AUDIO_H
